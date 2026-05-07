#include "FileProcessor.h"

#include <filesystem>

// Функция для конвертации XLSX в CSV
bool FileProcessor::convert_xlsx_to_csv(const std::string &xlsx_file) const
{
    for (auto &conv : converters)
    {
        if (!conv->is_available())
        {
            continue;
        }

        if (conv->xlsx_to_csv(xlsx_file))
        {
            return true;
        }
    }
    return false;
}

// Функция для конвертации CSV в XLSX
bool FileProcessor::convert_csv_to_xlsx(const std::string &csv_file) const
{
    for (auto &conv : converters)
    {
        if (!conv->is_available())
        {
            continue;
        }

        if (conv->csv_to_xlsx(csv_file))
        {
            return true;
        }
    }
    return false;
}

// Функция для удаления файлов
void FileProcessor::delete_file(const std::string &filename) const
{
    std::remove(filename.c_str());
}