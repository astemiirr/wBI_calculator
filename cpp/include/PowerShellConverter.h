#ifndef POWER_SHELL_CONVERTER_H
#define POWER_SHELL_CONVERTER_H

#include "IFileConverter.h"

class PowerShellConverter : public IFileConverter
{
private:
    std::string get_current_directory();

public:
    bool is_available() const override;
    bool xlsx_to_csv(const std::string &file) override;
    bool csv_to_xlsx(const std::string &file) override;
};

#endif