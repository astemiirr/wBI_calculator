#include "WbiCalculator.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <iomanip>

// Функция для вычисления условной суммы группы
ll WbiCalculator::group_cond_sum(ll group_sum, ll node, const std::vector<ll> &quota)
{

    // ll group_size = group.size();
    // ll sum = 0;
    // for (ll i = 0; i < group_size; ++i) {
    //     sum += in_weight[node][group[i]];
    // }

    if (group_sum >= quota[node])
    {
        return group_sum;
    }

    return 0;
}

// Рекурсивная функция для генерации комбинаций
void WbiCalculator::combination(ll curr, ll last_ind, ll count, ll group_sum, ll node, int option,
                                const std::vector<std::vector<ll>> &in_weight,
                                const std::vector<std::vector<ll>> &in_nodes,
                                const std::vector<ll> &quota,
                                const std::vector<ll> &in_edges_sum,
                                ll TOTAL_SUM,
                                std::vector<double> &wBI_1,
                                std::vector<double> &wBI_2)
{
    if (curr == count)
    {
        if (option == 1)
        {
            wBI_1[node] += double(group_cond_sum(group_sum, node, quota)) / in_edges_sum[node];
        }
        else if (option == 2)
        {
            wBI_2[node] += double(group_cond_sum(group_sum, node, quota)) / TOTAL_SUM;
        }
        else
        {
            wBI_1[node] += double(group_cond_sum(group_sum, node, quota)) / in_edges_sum[node];
            wBI_2[node] += double(group_cond_sum(group_sum, node, quota)) / TOTAL_SUM;
        }

        return;
    }

    ll in_count = in_nodes[node].size();
    if (count - curr > in_count - 1 - last_ind)
    {
        return;
    }

    for (ll i = last_ind + 1; i < in_count; ++i)
    {
        group_sum += in_weight[node][in_nodes[node][i]];
        combination(curr + 1, i, count, group_sum, node, option,
                    in_weight, in_nodes, quota, in_edges_sum, TOTAL_SUM, wBI_1, wBI_2);
        group_sum -= in_weight[node][in_nodes[node][i]];
    }

    return;
}

// Функция для вычисления wBI_1 и wBI_2 для одного узла
void WbiCalculator::calculate_wBI(ll node, int K, int option,
                                  const std::vector<std::vector<ll>> &in_weight,
                                  const std::vector<std::vector<ll>> &in_nodes,
                                  const std::vector<ll> &quota,
                                  const std::vector<ll> &in_edges_sum,
                                  ll TOTAL_SUM,
                                  std::vector<double> &wBI_1,
                                  std::vector<double> &wBI_2)
{
    for (ll i = 1; i <= K; ++i)
    {
        ll group_sum = 0;
        combination(0, -1, i, group_sum, node, option,
                    in_weight, in_nodes, quota, in_edges_sum, TOTAL_SUM, wBI_1, wBI_2);
    }
}
// Функция для вычисления сумм
ll WbiCalculator::calculate_sums(int N,
                                 const std::vector<std::vector<ll>> &in_weight,
                                 std::vector<ll> &in_edges_sum)
{
    ll sum = 0;
    in_edges_sum.assign(N, 0);

    for (ll i = 0; i < N; ++i)
    {
        for (ll j = 0; j < N; ++j)
        {
            in_edges_sum[i] += in_weight[i][j];
            sum += in_weight[i][j];
        }
    }
    return sum;
}

// Функция для выполнения вычислений
WbiResult WbiCalculator::perform_calculations(
    const GraphData &graph, const std::string &mode, const std::string &metric)
{
    // Переменные для вычислений
    const std::vector<std::vector<ll>> &in_weight = graph.get_in_weight();
    const std::vector<std::vector<ll>> &in_nodes = graph.get_in_nodes();
    const std::vector<ll> &quota = graph.get_quota();
    std::vector<ll> in_edges_sum;
    std::vector<double> wBI_1;
    std::vector<double> wBI_2;
    const std::vector<std::string> country_names = graph.get_country_names();
    int K = graph.get_K();

    int N = graph.size();

    int option;
    if (metric == "wbi1")
    {
        option = 1;
    }
    else if (metric == "wbi2")
    {
        option = 2;
    }
    else if (metric == "both")
    {
        option = 3;
    }
    else
    {
        throw std::invalid_argument("Metric must be \"wbi1\", \"wbi2\" or \"both\"");
    }

    ll TOTAL_SUM = calculate_sums(N, in_weight, in_edges_sum);

    // Инициализируем результаты
    wBI_1.assign(N, 0.0);
    wBI_2.assign(N, 0.0);

    // Параллельные вычисления
    auto start_time = std::chrono::high_resolution_clock::now();
    std::cout << "Calculating wBI_1 and wBI_2...\n"
              << std::endl;

    if (mode == "parallel")
    {
#pragma omp parallel for
        for (ll i = 0; i < N; ++i)
        {
            // std::cout << "Thread " << omp_get_thread_num() << " processing vertex " << i << std::endl;
            // std::cout << "Calculating for " << country_names[i] << "..." << std::endl;
            calculate_wBI(i, K, option, in_weight, in_nodes, quota, in_edges_sum, TOTAL_SUM, wBI_1, wBI_2);
        }
    }
    else if (mode == "linear")
    {
        for (ll i = 0; i < N; ++i)
        {
            // std::cout << "Thread " << omp_get_thread_num() << " processing vertex " << i << std::endl;
            // std::cout << "Calculating for " << country_names[i] << "..." << std::endl;
            calculate_wBI(i, K, option, in_weight, in_nodes, quota, in_edges_sum, TOTAL_SUM, wBI_1, wBI_2);
        }
    }
    else
    {
        throw std::invalid_argument("mode must be \"parallel\" or \"linear\"");
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto parallel_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << "ПАРАЛЛЕЛЬНОЕ ВЫЧИСЛЕНИЕ:" << "\n";
    std::cout << "Время: " << parallel_duration.count() << " мс" << "\n";
    std::cout << "\n";

    return WbiResult(wBI_1, wBI_2, country_names);
}