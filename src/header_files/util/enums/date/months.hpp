#include <map>
#include <string>

#ifndef MONTHENUMS_HPP
#define MONTHENUMS_HPP
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

        extern std::map<Month, std::string> monthsMap;

        extern Month allMonths[12];
    };
    int asInt(enums::Month month);

    enums::Month toMonth(int month);

    std::string getMonthName(enums::Month month);

    std::string getMonthName(int month);
};

#endif