#ifndef FILE_PROCESSOR_H
#define FILE_PROCESSOR_H

#include <bits/stdc++.h>
#include <chrono>
#include <omp.h>

// Класс для работы с файлами
class FileProcessor {
private:
    // Проверка доступности команды в системе
    bool command_exists(const std::string& command);

    // Проверка доступности Python
    bool python_available();
    
    // Проверка доступности LibreOffice
    bool libreoffice_available();
    
    // Конвертация XLSX в CSV через Python (кросс-платформенная)
    bool convert_xlsx_to_csv_python(const std::string& xlsx_file);

    // Конвертация CSV в XLSX через Python
    bool convert_csv_to_xlsx_python(const std::string& csv_file);

    // Конвертация XLSX в CSV через LibreOffice
    bool convert_xlsx_to_csv_libreoffice(const std::string& xlsx_file);

    // Конвертация CSV в XLSX через LibreOffice
    bool convert_csv_to_xlsx_libreoffice(const std::string& csv_file);

public:
    // Функция для конвертации XLSX в CSV (кросс-платформенная)
    bool convert_xlsx_to_csv(const std::string& xlsx_file);

    // Функция для конвертации CSV в XLSX (кросс-платформенная)
    bool convert_csv_to_xlsx(const std::string& csv_file);

    // Функция для удаления файлов
    void delete_file(const std::string& filename);
};

#endif