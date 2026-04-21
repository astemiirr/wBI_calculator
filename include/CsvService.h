#ifndef CSV_SERVICE_H
#define CSV_SERVICE_H

#include "GraphData.h"
#include "WbiResult.h"

#include <string>
#include <vector>
#include <map>
#include <stdexcept>

typedef long long ll;

class csv_service_error : public std::runtime_error
{
public:
    explicit csv_service_error(const std::string &msg)
        : std::runtime_error(msg) {}
};

class CsvService
{
public:
    // Функция для чтения рёбер из CSV
    void read_edges_csv(const std::string &filename,
                        int N,
                        std::vector<std::vector<ll>> &in_weight,
                        std::vector<std::vector<ll>> &in_nodes,
                        std::map<std::string, ll> &country_to_index);

    // Функция для чтения квот из CSV
    void read_quotas_csv(const std::string &filename,
                         std::vector<std::string> &country_names,
                         std::map<std::string, ll> &country_to_index,
                         std::vector<ll> &quota);

    GraphData load_graph(const std::string &quotas_file, const std::string &edges_file, ll K);

    // Функция для записи результатов в CSV
    void write_results_csv(const std::string &filename, const WbiResult &wbiResult);
};

#endif