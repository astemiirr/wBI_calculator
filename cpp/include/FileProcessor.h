#ifndef FILE_PROCESSOR_H
#define FILE_PROCESSOR_H

#include "IFileConverter.h"

#include <string>
#include <vector>
#include <memory>

typedef long long ll;

// Класс для работы с файлами
class FileProcessor
{
private:
    std::vector<std::unique_ptr<IFileConverter>> converters;

public:
    explicit FileProcessor(std::vector<std::unique_ptr<IFileConverter>> cons)
        : converters(std::move(cons)) {}

    // Функция для конвертации XLSX в CSV
    bool convert_xlsx_to_csv(const std::string &xlsx_file) const;

    // Функция для конвертации CSV в XLSX
    bool convert_csv_to_xlsx(const std::string &csv_file) const;

    // Функция для удаления файлов
    void delete_file(const std::string &filename) const;
};

#endif