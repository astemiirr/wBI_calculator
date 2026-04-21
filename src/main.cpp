#include "FileProcessor.h"
#include "WbiCalculator.h"
#include "CsvService.h"
#include "GraphData.h"
#include "WbiResult.h"

#include <iostream>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: wbi <K>";
        return 1;
    }

    int K = std::stoi(argv[1]);

    WbiCalculator calculator;
    FileProcessor fileProcessor;
    CsvService csv;

    // Конвертируем XLSX в CSV
    std::cout << "Converting quotas.xlsx to quotas.csv..." << std::endl;
    if (!fileProcessor.convert_xlsx_to_csv("quotas.xlsx"))
    {
        std::cerr << "Error converting quotas.xlsx to CSV!" << std::endl;
        return 1;
    }

    std::cout << "Converting network.xlsx to network.csv..." << std::endl;
    if (!fileProcessor.convert_xlsx_to_csv("network.xlsx"))
    {
        std::cerr << "Error converting network.xlsx to CSV!" << std::endl;
        return 1;
    }

    GraphData graph;

    try
    {
        graph = csv.load_graph("quotas.csv", "network.csv", K);
    }
    catch (std::runtime_error re)
    {
        std::cout << "Warning: " << re.what();
        return 1;
    }

    WbiResult result;
    try
    {
        // Выполняем вычисления индексов
        result = calculator.perform_calculations(graph);
    }
    catch (std::runtime_error re)
    {
        std::cout << "Warning: " << re.what();
        return 1;
    }

    std::cout << "Writing results to results.csv..." << std::endl;
    try
    {
        csv.write_results_csv("results.csv", result);
    }
    catch (std::runtime_error re)
    {
        std::cout << "Warning: " << re.what();
        return 1;
    }

    // Конвертируем результаты в XLSX
    std::cout << "Converting results.csv to results.xlsx..." << std::endl;
    if (!fileProcessor.convert_csv_to_xlsx("results.csv"))
    {
        std::cerr << "Error converting results.csv to XLSX!" << std::endl;
        return 1;
    }

    // Удаляем временные CSV файлы
    std::cout << "Cleaning up temporary files..." << std::endl;
    fileProcessor.delete_file("network.csv");
    fileProcessor.delete_file("quotas.csv");
    fileProcessor.delete_file("results.csv");

    std::cout << "Number of countries: " << graph.size() << std::endl;
    std::cout << "\nDone! Results saved to results.xlsx" << std::endl
              << std::endl;

    return 0;
}