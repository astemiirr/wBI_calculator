#ifndef WBI_RESULT_H
#define WBI_RESULT_H

#include <vector>
#include <string>

class WbiResult
{
private:
    std::vector<double> wBI_1;
    std::vector<double> wBI_2;
    std::vector<std::string> country_names;

public:
    WbiResult() = default;
    WbiResult(std::vector<double> wbi_1,
              std::vector<double> wbi_2,
              std::vector<std::string> cn) : wBI_1(wbi_1), wBI_2(wbi_2), country_names(cn) {}

    const std::vector<std::string> &get_country_names() const
    {
        return country_names;
    }

    const std::vector<double> &get_wbi_1() const
    {
        return wBI_1;
    }

    const std::vector<double> &get_wbi_2() const
    {
        return wBI_2;
    }
};

#endif