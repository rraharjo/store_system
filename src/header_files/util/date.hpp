#include <chrono>
#include <string>

#ifndef DATEUTIL
#define DATEUTIL

namespace util
{
    class Date
    {
    private:
        std::chrono::system_clock::time_point tp;
        std::chrono::system_clock::time_point parseStringDate(std::string dateString, std::string format);
        std::chrono::system_clock::time_point parseStringDateddMMYYYYseparetedBySlash(std::string dateString);

    public:
        Date();
        Date(std::string dateString);
        Date(std::string dateString, std::string format);
        std::string to_string();
        std::string toDBFormat();
        bool isLeapYear();
        int getYDay();
        int getMDay();
        int getMonth();
        int getYear();
        bool isAfter(util::Date *other);
        int diffDaysTo(util::Date *other);
        int diffYearsTo(util::Date *other);
        bool operator==(const Date *other);
        bool operator!=(const Date *other);
        bool operator<(const Date *other);
        bool operator>(const Date *other);
        bool operator<=(const Date *other);
        bool operator>=(const Date *other);

        static time_t getUniqueTimestamp();
    };
};
#endif
