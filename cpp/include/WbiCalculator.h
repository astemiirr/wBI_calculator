#ifndef WBI_CALCULATOR_H
#define WBI_CALCULATOR_H

#include "GraphData.h"
#include "WbiResult.h"

#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <omp.h>

typedef long long ll;

// Класс для вычисления индексов wBI
class WbiCalculator
{
private:
    // Функция для вычисления условной суммы группы
    ll group_cond_sum(ll group_sum, ll node, const std::vector<ll> &quota);

    // Рекурсивная функция для генерации комбинаций
    void combination(ll curr, ll last_ind, ll count, ll group_sum, ll node, int option,
                     const std::vector<std::vector<ll>> &in_weight,
                     const std::vector<std::vector<ll>> &in_nodes,
                     const std::vector<ll> &quota,
                     const std::vector<ll> &in_edges_sum,
                     ll TOTAL_SUM,
                     std::vector<double> &wBI_1,
                     std::vector<double> &wBI_2);

    // Функция для вычисления wBI_1 и wBI_2 для одного узла
    void calculate_wBI(ll node, int K, int option,
                       const std::vector<std::vector<ll>> &in_weight,
                       const std::vector<std::vector<ll>> &in_nodes,
                       const std::vector<ll> &quota,
                       const std::vector<ll> &in_edges_sum,
                       ll TOTAL_SUM,
                       std::vector<double> &wBI_1,
                       std::vector<double> &wBI_2);

    // Функция для вычисления сумм
    ll calculate_sums(int N,
                      const std::vector<std::vector<ll>> &in_weight,
                      std::vector<ll> &in_edges_sum);

public:
    WbiCalculator() = default;

    // Функция для выполнения вычислений
    WbiResult perform_calculations(const GraphData &graph, const std::string &mode, const std::string &metric);
};

#endif