#include <chrono>
#include <string>
#include <regex>

#ifndef DATEUTIL
#define DATEUTIL

namespace util
{
    class Date
    {
    private:
        std::chrono::system_clock::time_point tp;
        std::chrono::system_clock::time_point parse_string_date(std::string date_string, std::string format);
        std::chrono::system_clock::time_point parse_string_date_dd_MM_YYYY_separeted_by_slash(std::string date_string);

    public:
        Date();
        Date(std::string date_string);
        Date(std::string date_string, std::string format);
        std::string to_string();
        std::string to_db_format();
        bool is_leap_year();
        int get_y_day();
        int get_m_day();
        int get_month();
        int get_year();
        bool is_after(util::Date *other);
        int diff_days_to(util::Date *other);
        int diff_years_to(util::Date *other);
        bool operator==(const Date *other);
        bool operator!=(const Date *other);
        bool operator<(const Date *other);
        bool operator>(const Date *other);
        bool operator<=(const Date *other);
        bool operator>=(const Date *other);

        static time_t get_unique_timestamp();
    };
};
#endif
