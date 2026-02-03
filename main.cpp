#include "wbi_indices.h"

int main() {
    WbiCalculator calculator;
    FileProcessor fileProcessor;
    
    std::cout << "Enter parameter K: ";
    std::cin >> calculator.K;
    
    // Шаг 1: Конвертируем XLSX в CSV
    std::cout << "Converting quotas.xlsx to quotas.csv..." << std::endl;
    // if (!fileProcessor.convert_xlsx_to_csv("quotas.xlsx")) {
    //     std::cerr << "Error converting quotas.xlsx to CSV!" << std::endl;
    //     return 1;
    // }
    
    // std::cout << "Converting network.xlsx to network.csv..." << std::endl;
    // if (!fileProcessor.convert_xlsx_to_csv("network.xlsx")) {
    //     std::cerr << "Error converting network.xlsx to CSV!" << std::endl;
    //     return 1;
    // }
    
    // Шаг 2: Выполняем вычисления индексов
    calculator.perform_calculations();
    
    // Шаг 3: Конвертируем результаты в XLSX
    std::cout << "Converting results.csv to results.xlsx..." << std::endl;
    if (!fileProcessor.convert_csv_to_xlsx("results.csv")) {
        std::cerr << "Error converting results.csv to XLSX!" << std::endl;
        return 1;
    }
    
    // Шаг 4: Удаляем временные CSV файлы
    std::cout << "Cleaning up temporary files..." << std::endl;
    // fileProcessor.delete_file("network.csv");
    // fileProcessor.delete_file("quotas.csv");
    fileProcessor.delete_file("results.csv");
    
    std::cout << "Number of countries: " << calculator.N << std::endl;
    std::cout << "Total sum of weights: " << calculator.TOTAL_SUM << std::endl;
    std::cout << "\nDone! Results saved to results.xlsx" << std::endl << std::endl;
    
    return 0;
}