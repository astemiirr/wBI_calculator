#include "PythonConverter.h"

#include <vector>

bool PythonConverter::command_exists(const std::string &command) const
{
#ifdef _WIN32
    std::string test_cmd = "where " + command + " > nul 2>&1";
#else
    std::string test_cmd = "command -v " + command + " > /dev/null 2>&1";
#endif
    return (system(test_cmd.c_str()) == 0);
}

bool PythonConverter::is_available() const
{
    return command_exists("python") || command_exists("python3");
}

bool PythonConverter::csv_to_xlsx(const std::string &file)
{
    std::string base_name = file.substr(0, file.find_last_of('.'));
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
                file + "').to_excel(r'" + xlsx_file + "', index=False)\"";

            int result = system(command.c_str());
            if (result == 0)
            {
                return true;
            }
        }
    }
    return false;
}

bool PythonConverter::xlsx_to_csv(const std::string &file)
{

    std::string base_name = file.substr(0, file.find_last_of('.'));
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
                file + "').to_csv(r'" + csv_file + "', index=False)\"";

            int result = system(command.c_str());
            if (result == 0)
            {
                return true;
            }
        }
    }
    return false;
}