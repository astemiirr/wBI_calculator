#include "CsvService.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <iomanip>

#include <iostream>

// Функция для чтения рёбер из CSV
void CsvService::read_edges_csv(const std::string &filename,
                                int N,
                                std::vector<std::vector<ll>> &in_weight,
                                std::vector<std::vector<ll>> &in_nodes,
                                std::map<std::string, size_t> &country_to_index)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw csv_service_error("Failed to open file: " + filename);
    }

    std::string line;

    // Инициализируем структуры данных
    in_weight.assign(N, std::vector<ll>(N, 0));
    in_nodes.resize(N);

    // Читаем заголовок (пропускаем)
    std::getline(file, line);

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string from_country, to_country, weight_str;

        // Читаем три столбца: from_country; to_country; weight
        // Читаем три поля, разделенные ;

        if (std::getline(ss, from_country, ';') &&
            std::getline(ss, to_country, ';') &&
            std::getline(ss, weight_str, ';'))
        {
            // Убираем возможные пробелы
            from_country.erase(0, from_country.find_first_not_of(" \t"));
            from_country.erase(from_country.find_last_not_of(" \t") + 1);
            to_country.erase(0, to_country.find_first_not_of(" \t"));
            to_country.erase(to_country.find_last_not_of(" \t") + 1);
            weight_str.erase(0, weight_str.find_first_not_of(" \t"));
            weight_str.erase(weight_str.find_last_not_of(" \t") + 1);

            // Проверяем, что страны есть в нашем списке
            if (country_to_index.count(from_country) && country_to_index.count(to_country))
            {
                ll from_idx = country_to_index[from_country];
                ll to_idx = country_to_index[to_country];
                ll weight = std::stoll(weight_str);

                in_weight[to_idx][from_idx] = weight;
                in_nodes[to_idx].push_back(from_idx);
            }

            else
            {
                throw csv_service_error("Warning: Unknown country in edge: " + from_country + " -> " + to_country);
            }
        }
        else
        {
            throw csv_service_error("Warning: Could not parse line: " + line);
        }
    }

    // Нахождение узла с максимальным количеством входящих рёбер
    ll mx_in_nodes = 0;
    for (auto &el : in_nodes)
    {
        mx_in_nodes = fmax(mx_in_nodes, el.size()); // ll -> double
    }
    std::cout << std::endl
              << "Maximum number of incoming edges = " << mx_in_nodes << std::endl;
    std::cout << "-------------------------" << std::endl
              << std::endl;

    file.close();
}

// Функция для чтения квот из CSV
void CsvService::read_quotas_csv(const std::string &filename,
                                 std::vector<std::string> &country_names,
                                 std::map<std::string, size_t> &country_to_index,
                                 std::vector<ll> &quota)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw csv_service_error("Failed to open file: " + filename);
    }

    std::string line;

    // // Читаем заголовок (пропускаем)
    std::getline(file, line);

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string country_name, quota_str;

        // Читаем два поля, разделенные ;
        if (std::getline(ss, country_name, ';') && std::getline(ss, quota_str, ';'))
        {
            // Убираем возможные пробелы
            country_name.erase(0, country_name.find_first_not_of(" \t"));
            country_name.erase(country_name.find_last_not_of(" \t") + 1);
            quota_str.erase(0, quota_str.find_first_not_of(" \t"));
            quota_str.erase(quota_str.find_last_not_of(" \t") + 1);

            if (!country_to_index.count(country_name))
            {
                country_to_index[country_name] = country_names.size();
                country_names.push_back(country_name);

                quota.push_back(std::stoll(quota_str));
            }
            else if (std::stoll(quota_str) != quota[country_to_index[country_name]])
            {
                throw csv_service_error("Warning: Two different quotas per country");
            }
        }
        else
        {
            throw csv_service_error("Warning: Could not parse quotas line: " + line);
        }
    }

    file.close();
}

GraphData CsvService::load_graph(const std::string &quotas_file, const std::string &edges_file, ll K)
{
    std::vector<std::string> country_names;
    std::map<std::string, size_t> country_to_index;
    std::vector<ll> quota;

    read_quotas_csv(quotas_file, country_names, country_to_index, quota);

    std::vector<std::vector<ll>> in_weight;
    std::vector<std::vector<ll>> in_nodes;
    read_edges_csv(edges_file, quota.size(), in_weight, in_nodes, country_to_index);

    return GraphData(K, in_weight, in_nodes, quota, country_names, country_to_index);
}

// Функция для записи результатов в CSV
void CsvService::write_results_csv(const std::string &filename, const WbiResult &wbiResult)
{
    const std::vector<std::string> &country_names = wbiResult.get_country_names();
    std::vector<double> wBI_1 = wbiResult.get_wbi_1();
    std::vector<double> wBI_2 = wbiResult.get_wbi_2();

    std::ofstream file(filename);

    if (!file.is_open())
    {
        throw csv_service_error("Failed to open file: " + filename);
    }

    // Заголовки
    file << "Country;wBI_1;wBI_2\n";

    // Данные - используем исходные названия стран
    for (ll i = 0; i < country_names.size(); ++i)
    {
        file << country_names[i] << ";"
             << std::fixed << std::setprecision(6) << wBI_1[i] << ";"
             << std::fixed << std::setprecision(6) << wBI_2[i] << "\n";
    }

    file.close();
}
