#ifndef GRAPH_DATA_H
#define GRAPH_DATA_H

#include <vector>
#include <string>
#include <map>
#include <iostream>

#include <stdexcept>

typedef double db;

class graph_data_error : public std::runtime_error
{
public:
    explicit graph_data_error(const std::string &msg)
        : std::runtime_error(msg) {}
};

class GraphData
{
private:
    // Переменные для вычислений
    std::vector<std::vector<db>> in_weight;    // in_weight[x][y] вес ребра x <-- y
    std::vector<std::vector<size_t>> in_nodes; // in_nodes[x] список ребер входящие в x
    std::vector<db> quota;                     // квота для каждой вершины

    // Векторы для работы со странами
    std::vector<std::string> country_names;         // country_names[i] = название страны с индексом i
    std::map<std::string, size_t> country_to_index; // country_to_index["Russia"] = индекс

    size_t K;

public:
    GraphData() = default;
    GraphData(
        size_t k,
        const std::vector<std::vector<db>> &in_weight,
        const std::vector<std::vector<size_t>> &in_nodes,
        const std::vector<db> &quota,
        const std::vector<std::string> &country_names,
        const std::map<std::string, size_t> &country_to_index) : K(k),
                                                                 in_weight(in_weight),
                                                                 in_nodes(in_nodes),
                                                                 quota(quota),
                                                                 country_names(country_names),
                                                                 country_to_index(country_to_index)
    {
        if (in_weight.size() != in_nodes.size() ||
            in_weight.size() != country_names.size() ||
            in_weight.size() != quota.size())
        {
            std::cout << "Invalid GraphData sizes !";
            throw graph_data_error("Invalid GraphData sizes!");
        }
    }

    size_t size() const
    {
        return in_weight.size();
    }

    size_t get_K() const
    {
        return K;
    }

    const std::vector<std::vector<db>> &get_in_weight() const
    {
        return in_weight;
    }

    const std::vector<std::vector<size_t>> &get_in_nodes() const
    {
        return in_nodes;
    }

    const std::vector<db> &get_quota() const
    {
        return quota;
    }

    const std::vector<std::string> &get_country_names() const
    {
        return country_names;
    }
};

#endif