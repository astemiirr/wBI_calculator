#ifndef WBI_CALCULATOR_H
#define WBI_CALCULATOR_H

#include <bits/stdc++.h>
#include <chrono>
#include <omp.h>

typedef long long ll;

// Класс для вычисления индексов wBI
class WbiCalculator
{
private:
    // Переменные для вычислений
    std::vector<std::vector<ll>> in_weight;
    std::vector<std::vector<ll>> in_nodes;
    std::vector<ll> quota;
    std::vector<ll> in_edges_sum;
    std::vector<double> wBI_1;
    std::vector<double> wBI_2;

    // Векторы для работы со странами
    std::vector<std::string> country_names;     // country_names[i] = название страны с индексом i
    std::map<std::string, ll> country_to_index; // country_to_index["Russia"] = индекс

    // Функция для вычисления условной суммы группы
    ll group_cond_sum(ll group_sum, ll node);

    // Рекурсивная функция для генерации комбинаций
    void combination(ll curr, ll last_ind, ll count, ll group_sum, ll node, ll option);

    // Функция для вычисления wBI_1 и wBI_2 для одного узла
    void calculate_wBI(ll node);

    // Функция для чтения списка стран из квот
    void read_countries_from_quotas_csv(const std::string &filename);

    // Функция для чтения рёбер из CSV
    void read_edges_csv(const std::string &filename);

    // Функция для чтения квот из CSV
    void read_quotas_csv(const std::string &filename);

    // Функция для вычисления сумм
    void calculate_sums();

    // Функция для записи результатов в CSV
    void write_results_csv(const std::string &filename);

public:
    ll K;
    ll N;
    ll TOTAL_SUM;

    WbiCalculator(int K) : K(K), N(0), TOTAL_SUM(0) {}

    // Функция для выполнения вычислений
    void perform_calculations();
};

#endif