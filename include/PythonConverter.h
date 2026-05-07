#ifndef PYTHON_CONVERTER_H
#define PYTHON_CONVERTER_H

#include "IFileConverter.h"

class PythonConverter : public IFileConverter
{
private:
    bool command_exists(const std::string &command) const;

public:
    bool is_available() const override;
    bool xlsx_to_csv(const std::string &file) override;
    bool csv_to_xlsx(const std::string &file) override;
};

#endif