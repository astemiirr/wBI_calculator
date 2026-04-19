#include "FileProcessor.h"

#include <filesystem>
#include <chrono>
#include <omp.h>
#include <vector>
#include <iostream>

// Проверка доступности команды в системе
bool FileProcessor::command_exists(const std::string &command)
{
#ifdef _WIN32
    std::string test_cmd = "where " + command + " > nul 2>&1";
#else
    std::string test_cmd = "command -v " + command + " > /dev/null 2>&1";
#endif
    return (system(test_cmd.c_str()) == 0);
}

// Проверка доступности Python
bool FileProcessor::python_available()
{
    return command_exists("python") || command_exists("python3");
}

// Проверка доступности LibreOffice
bool FileProcessor::libreoffice_available()
{
    return command_exists("libreoffice");
}

// Получение пути к текущей директории
std::string FileProcessor::get_current_directory()
{
    return std::filesystem::current_path().string();
}

// Конвертация XLSX в CSV через PowerShell (Windows)
bool FileProcessor::convert_xlsx_to_csv_powershell(const std::string &xlsx_file)
{
    std::string base_name = xlsx_file.substr(0, xlsx_file.find_last_of('.'));
    std::string csv_file = base_name + ".csv";
    std::string current_dir = get_current_directory();

    std::string command = "powershell -Command \""
                          "$excel = New-Object -ComObject Excel.Application; "
                          "$excel.Visible = $false; "
                          "$excel.DisplayAlerts = $false; "
                          "$workbook = $excel.Workbooks.Open('" +
                          current_dir + "\\" + xlsx_file + "'); "
                                                           "$workbook.SaveAs('" +
                          current_dir + "\\" + csv_file + "', 6); " // 6 = xlCSV
                                                          "$workbook.Close(); "
                                                          "$excel.Quit(); "
                                                          "[System.Runtime.Interopservices.Marshal]::ReleaseComObject($excel) | Out-Null; "
                                                          "if (Test-Path '" +
                          current_dir + "\\" + csv_file + "') { exit 0 } else { exit 1 }\"";

    int result = system(command.c_str());
    return (result == 0);
}

// Конвертация CSV в XLSX через PowerShell (Windows)
bool FileProcessor::convert_csv_to_xlsx_powershell(const std::string &csv_file)
{
    std::string base_name = csv_file.substr(0, csv_file.find_last_of('.'));
    std::string xlsx_file = base_name + ".xlsx";
    std::string current_dir = get_current_directory();

    std::string command = "powershell -Command \""
                          "$excel = New-Object -ComObject Excel.Application; "
                          "$excel.Visible = $false; "
                          "$excel.DisplayAlerts = $false; "
                          "$workbook = $excel.Workbooks.Open('" +
                          current_dir + "\\" + csv_file + "'); "
                                                          "$workbook.SaveAs('" +
                          current_dir + "\\" + xlsx_file + "', 51); " // 51 = xlOpenXMLWorkbook
                                                           "$workbook.Close(); "
                                                           "$excel.Quit(); "
                                                           "[System.Runtime.Interopservices.Marshal]::ReleaseComObject($excel) | Out-Null; "
                                                           "if (Test-Path '" +
                          current_dir + "\\" + xlsx_file + "') { exit 0 } else { exit 1 }\"";

    int result = system(command.c_str());
    return (result == 0);
}

// Конвертация XLSX в CSV через Python
bool FileProcessor::convert_xlsx_to_csv_python(const std::string &xlsx_file)
{
    std::string base_name = xlsx_file.substr(0, xlsx_file.find_last_of('.'));
    std::string csv_file = base_name + ".csv";

    // Пробуем сначала python, потом python3
    std::vector<std::string> python_cmds = {"python", "python3"};

    for (const auto &python_cmd : python_cmds)
    {
        if (command_exists(python_cmd))
        {
            std::string command =
                python_cmd + " -c \"import pandas as pd;"
                             "pd.read_excel(r'" +
                xlsx_file + "').to_csv(r'" + csv_file + "', index=False)\"";

            int result = system(command.c_str());
            if (result == 0)
            {
                return true;
            }
        }
    }
    return false;
}

// Конвертация CSV в XLSX через Python
bool FileProcessor::convert_csv_to_xlsx_python(const std::string &csv_file)
{
    std::string base_name = csv_file.substr(0, csv_file.find_last_of('.'));
    std::string xlsx_file = base_name + ".xlsx";

    // Пробуем сначала python, потом python3
    std::vector<std::string> python_cmds = {"python", "python3"};

    for (const auto &python_cmd : python_cmds)
    {
        if (command_exists(python_cmd))
        {
            std::string command =
                python_cmd + " -c \"import pandas as pd;"
                             "pd.read_csv(r'" +
                csv_file + "').to_excel(r'" + xlsx_file + "', index=False)\"";

            int result = system(command.c_str());
            if (result == 0)
            {
                return true;
            }
        }
    }
    return false;
}

// Конвертация XLSX в CSV через LibreOffice
bool FileProcessor::convert_xlsx_to_csv_libreoffice(const std::string &xlsx_file)
{
    std::string command = "libreoffice --headless --convert-to csv " + xlsx_file + " --outdir . > /dev/null 2>&1";
    int result = system(command.c_str());
    return (result == 0);
}

// Конвертация CSV в XLSX через LibreOffice
bool FileProcessor::convert_csv_to_xlsx_libreoffice(const std::string &csv_file)
{
    std::string command = "libreoffice --headless --convert-to xlsx " + csv_file + " --outdir . > /dev/null 2>&1";
    int result = system(command.c_str());
    return (result == 0);
}

bool FileProcessor::convert_xlsx_to_csv(const std::string &xlsx_file)
{
#ifdef _WIN32
    // Windows: сначала пробуем PowerShell, потом Python
    if (convert_xlsx_to_csv_powershell(xlsx_file))
    {
        std::cout << "Successfully converted using PowerShell" << std::endl;
        return true;
    }

    std::cout << "PowerShell failed, trying Python..." << std::endl;
    if (python_available())
    {
        if (convert_xlsx_to_csv_python(xlsx_file))
        {
            std::cout << "Successfully converted using Python" << std::endl;
            return true;
        }
    }

    std::cerr << "Error: Neither PowerShell nor Python available for conversion!" << std::endl;
    return false;
#else
    // Linux/Unix: сначала пробуем LibreOffice, потом Python
    if (libreoffice_available())
    {
        if (convert_xlsx_to_csv_libreoffice(xlsx_file))
        {
            std::cout << "Successfully converted using LibreOffice" << std::endl;
            return true;
        }
    }

    std::cout << "LibreOffice failed or not available, trying Python..." << std::endl;
    if (python_available())
    {
        if (convert_xlsx_to_csv_python(xlsx_file))
        {
            std::cout << "Successfully converted using Python" << std::endl;
            return true;
        }
    }

    std::cerr << "Error: Neither LibreOffice nor Python available for conversion!" << std::endl;
    return false;
#endif
}

// Функция для конвертации CSV в XLSX (кросс-платформенная)
bool FileProcessor::convert_csv_to_xlsx(const std::string &csv_file)
{
#ifdef _WIN32
    // Windows: сначала пробуем PowerShell, потом Python
    if (convert_csv_to_xlsx_powershell(csv_file))
    {
        std::cout << "Successfully converted using PowerShell" << std::endl;
        return true;
    }

    std::cout << "PowerShell failed, trying Python..." << std::endl;
    if (python_available())
    {
        if (convert_csv_to_xlsx_python(csv_file))
        {
            std::cout << "Successfully converted using Python" << std::endl;
            return true;
        }
    }

    std::cerr << "Error: Neither PowerShell nor Python available for conversion!" << std::endl;
    return false;
#else
    // Linux/Unix: сначала пробуем LibreOffice, потом Python
    if (libreoffice_available())
    {
        if (convert_csv_to_xlsx_libreoffice(csv_file))
        {
            std::cout << "Successfully converted using LibreOffice" << std::endl;
            return true;
        }
    }

    std::cout << "LibreOffice failed or not available, trying Python..." << std::endl;
    if (python_available())
    {
        if (convert_csv_to_xlsx_python(csv_file))
        {
            std::cout << "Successfully converted using Python" << std::endl;
            return true;
        }
    }

    std::cerr << "Error: Neither LibreOffice nor Python available for conversion!" << std::endl;
    return false;
#endif
}

// Функция для удаления файлов
void FileProcessor::delete_file(const std::string &filename)
{
    std::remove(filename.c_str());
}