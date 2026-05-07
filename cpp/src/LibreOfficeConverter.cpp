#include "LibreOfficeConverter.h"

bool LibreOfficeConverter::is_available() const
{
#ifdef _WIN32
    return false;
#else
    return system("command -v libreoffice > /dev/null 2>&1") == 0;
#endif
}

bool LibreOfficeConverter::xlsx_to_csv(const std::string &file)
{
    std::string command = "libreoffice --headless --convert-to csv " + file + " --outdir . > /dev/null 2>&1";
    int result = system(command.c_str());
    return (result == 0);
}

bool LibreOfficeConverter::csv_to_xlsx(const std::string &file)
{
    std::string command = "libreoffice --headless --convert-to xlsx " + file + " --outdir . > /dev/null 2>&1";
    int result = system(command.c_str());
    return (result == 0);
}