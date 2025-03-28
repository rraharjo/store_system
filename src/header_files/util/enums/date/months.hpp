#ifndef MONTHENUMS_HPP
#define MONTHENUMS_HPP
#include <map>
#include <string>


namespace util
{
    namespace enums
    {
        enum Month
        {
            JANUARY = 1,
            FEBRUARY = 2,
            MARCH = 3,
            APRIL = 4,
            MAY = 5,
            JUNE = 6,
            JULY = 7,
            AUGUST = 8,
            SEPTEMBER = 9,
            OCTOBER = 10,
            NOVEMBER = 11,
            DECEMBER = 12
        };

        extern std::map<Month, std::string> months_map;

        extern Month all_months[12];
    };
    int as_int(enums::Month month);

    enums::Month to_month(int month);

    std::string get_month_name(enums::Month month);

    std::string get_month_name(int month);
};

#endif