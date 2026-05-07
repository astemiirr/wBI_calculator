#ifndef LIBRE_CONVERTER_H
#define LIBRE_CONVERTER_H

#include "IFileConverter.h"

class LibreOfficeConverter : public IFileConverter
{
public:
    bool is_available() const override;
    bool xlsx_to_csv(const std::string &file) override;
    bool csv_to_xlsx(const std::string &file) override;
};

#endif