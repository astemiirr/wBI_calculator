#include "WbiCalculator.h"
#include "CsvService.h"
#include "GraphData.h"
#include "WbiResult.h"

#include <iostream>
#include <vector>
#include <memory>

int main(int argc, char *argv[])
{
    if (argc != 13)
    {
        std::cout << "Usage: wbi --quotas quotas.csv --network network.csv --output result.csv --k 2 --mode parallel --metric both";
        return 1;
    }
    int K = std::stoi(argv[8]);

    std::string quotas_csv = argv[2], network_csv = argv[4], result_csv = argv[6];

    std::string mode = argv[10], metric = argv[12];

    WbiCalculator calculator;
    CsvService csv;
    GraphData graph;

    try
    {
        graph = csv.load_graph(quotas_csv, network_csv, K);
    }
    catch (std::runtime_error re)
    {
        std::cerr << "Warning loading graph: " << re.what();
        return 1;
    }

    WbiResult result;
    try
    {
        // Выполняем вычисления индексов
        result = calculator.perform_calculations(graph, mode, metric);
    }
    catch (std::runtime_error re)
    {
        std::cerr << "Calculating error: " << re.what();
        return 1;
    }

    std::cout << "Writing results to " << result_csv << "..." << std::endl;
    try
    {
        csv.write_results_csv(result_csv, result);
    }
    catch (std::runtime_error re)
    {
        std::cerr << "Error output answer: " << re.what();
        return 1;
    }

    std::cout << "Number of countries: " << graph.size() << std::endl;
    std::cout << "\nDone! Results saved to results.xlsx" << std::endl
              << std::endl;

    return 0;
}