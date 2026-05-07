#ifndef WBI_CALCULATOR_H
#define WBI_CALCULATOR_H

#include "GraphData.h"
#include "WbiResult.h"

#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <omp.h>

typedef double db;

// Класс для вычисления индексов wBI
class WbiCalculator
{
private:
    // Функция для вычисления условной суммы группы
    db group_cond_sum(db group_sum, size_t node, const std::vector<db> &quota);

    // Рекурсивная функция для генерации комбинаций
    void combination(size_t curr, size_t last_ind, size_t count, db group_sum, size_t node,
                     int option,
                     const std::vector<std::vector<db>> &in_weight,
                     const std::vector<std::vector<size_t>> &in_nodes,
                     const std::vector<db> &quota,
                     const std::vector<db> &in_edges_sum,
                     db TOTAL_SUM,
                     std::vector<double> &wBI_1,
                     std::vector<double> &wBI_2);

    // Функция для вычисления wBI_1 и wBI_2 для одного узла
    void calculate_wBI(size_t node, size_t K, int option,
                       const std::vector<std::vector<db>> &in_weight,
                       const std::vector<std::vector<size_t>> &in_nodes,
                       const std::vector<db> &quota,
                       const std::vector<db> &in_edges_sum,
                       db TOTAL_SUM,
                       std::vector<db> &wBI_1,
                       std::vector<db> &wBI_2);

    // Функция для вычисления сумм
    db calculate_sums(size_t N,
                      const std::vector<std::vector<db>> &in_weight,
                      std::vector<db> &in_edges_sum);

public:
    WbiCalculator() = default;

    // Функция для выполнения вычислений
    WbiResult perform_calculations(const GraphData &graph, const std::string &mode, const std::string &metric);
};

#endif