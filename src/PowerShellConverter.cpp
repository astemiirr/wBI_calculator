#include "PowerShellConverter.h"

#include <chrono>
#include <filesystem>

std::string PowerShellConverter::get_current_directory()
{
    return std::filesystem::current_path().string();
}

bool PowerShellConverter::is_available() const
{
#ifdef _WIN32
    return true;
#else
    return false;
#endif
}

bool PowerShellConverter::xlsx_to_csv(const std::string &file)
{
    std::string base_name = file.substr(0, file.find_last_of('.'));
    std::string csv_file = base_name + ".csv";
    std::string current_dir = get_current_directory();

    std::string command = "powershell -Command \""
                          "$excel = New-Object -ComObject Excel.Application; "
                          "$excel.Visible = $false; "
                          "$excel.DisplayAlerts = $false; "
                          "$workbook = $excel.Workbooks.Open('" +
                          current_dir + "\\" + file + "'); "
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

bool PowerShellConverter::csv_to_xlsx(const std::string &file)
{
    std::string base_name = file.substr(0, file.find_last_of('.'));
    std::string xlsx_file = base_name + ".xlsx";
    std::string current_dir = get_current_directory();

    std::string command = "powershell -Command \""
                          "$excel = New-Object -ComObject Excel.Application; "
                          "$excel.Visible = $false; "
                          "$excel.DisplayAlerts = $false; "
                          "$workbook = $excel.Workbooks.Open('" +
                          current_dir + "\\" + file + "'); "
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