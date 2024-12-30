#include <sstream>
#include <iomanip>
#include "util/date.hpp"
#include "util/enums/date/months.hpp"

using namespace util;

Date::Date()
{
    this->tp = std::chrono::system_clock::now();
}

Date::Date(std::string date_string, std::string format)
{
    this->tp = this->parse_string_date(date_string, format);
}

Date::Date(std::string date_string)
{
    const std::regex regex_format("[0-3][0-9]/[0-1][0-9]/[0-9][0-9][0-9][0-9]");
    if (!std::regex_match(date_string, regex_format)){
        throw std::invalid_argument(date_string + " is not in dd/MM/YYYY format");
    }
    this->tp = this->parse_string_date_dd_MM_YYYY_separeted_by_slash(date_string);
}

std::chrono::system_clock::time_point Date::parse_string_date(std::string date_string, std::string format)
{
    std::tm tm = {};
    std::istringstream ss(date_string);
    ss >> std::get_time(&tm, format.c_str());
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

std::chrono::system_clock::time_point Date::parse_string_date_dd_MM_YYYY_separeted_by_slash(std::string date_string)
{
    return Date::parse_string_date(date_string, "%d/%m/%Y");
}

int Date::get_y_day()
{
    time_t tt = std::chrono::system_clock::to_time_t(this->tp);
    tm local = *localtime(&tt);
    return local.tm_yday + 1;
}

int Date::get_m_day()
{
    time_t tt = std::chrono::system_clock::to_time_t(this->tp);
    tm local = *localtime(&tt);
    return local.tm_mday;
}

int Date::get_month()
{
    time_t tt = std::chrono::system_clock::to_time_t(this->tp);
    tm local = *localtime(&tt);
    return local.tm_mon + 1;
}

int Date::get_year()
{
    time_t tt = std::chrono::system_clock::to_time_t(this->tp);
    tm local = *localtime(&tt);
    return local.tm_year + 1900;
}

bool Date::is_leap_year()
{
    if (this->get_year() % 100 == 0)
    {
        return this->get_year() % 400 == 0;
    }
    return this->get_year() % 4 == 0;
}

std::string Date::to_string()
{
    std::string to_ret = "";
    int year = this->get_year();
    int month = this->get_month();
    int month_day = this->get_m_day();
    to_ret += std::to_string(month_day) + "-";
    to_ret += util::get_month_name(month) + "-";
    to_ret += std::to_string(year);
    return to_ret;
}

std::string Date::to_db_format(){
    std::string to_ret = "";
    int year = this->get_year();
    int month = this->get_month();
    int month_day = this->get_m_day();
    to_ret += std::to_string(month_day) + "-";
    to_ret += std::to_string(month) + "-";
    to_ret += std::to_string(year);
    return to_ret;
}

bool Date::is_after(Date *other)
{
    return this->diff_days_to(other) < 0;
}

int Date::diff_days_to(Date *other) // negative if this is after other
{
    std::chrono::time_point other_tp = other->tp;
    std::chrono::duration duration_diff = other->tp - this->tp;
    auto second_diff = std::chrono::duration_cast<std::chrono::seconds>(duration_diff);
    return second_diff.count() / 60 / 60 / 24;
}

int Date::diff_years_to(Date *other) // negative if this is after other
{
    int years_diff;
    if (this->is_after(other))
    {
        years_diff = this->get_year() - other->get_year();
        if (this->get_y_day() < other->get_y_day())
        {
            years_diff -= 1;
        }
        years_diff = -1 * years_diff;
    }
    else
    {
        years_diff = other->get_year() - this->get_year();
        if (other->get_y_day() < this->get_y_day())
        {
            years_diff -= 1;
        }
    }
    return years_diff;
}

bool Date::operator==(const Date *other)
{
    return this->tp == other->tp;
}
bool Date::operator!=(const Date *other)
{
    return !(this == other);
}
bool Date::operator<(const Date *other)
{
    return this->tp < other->tp;
}
bool Date::operator>(const Date *other)
{
    return !(this < other);
}
bool Date::operator<=(const Date *other)
{
    return this < other || this == other;
}
bool Date::operator>=(const Date *other)
{
    return this > other || this == other;
}

time_t Date::get_unique_timestamp()
{
    const auto p1 = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(p1.time_since_epoch()).count();
}
