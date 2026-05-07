#ifndef IFILE_CONVERTER_H
#define IFILE_CONVERTER_H

#include <string>

class IFileConverter
{
public:
    virtual bool is_available() const = 0;
    virtual bool xlsx_to_csv(const std::string &) = 0;
    virtual bool csv_to_xlsx(const std::string &) = 0;
    virtual ~IFileConverter() = default;
};

#endif