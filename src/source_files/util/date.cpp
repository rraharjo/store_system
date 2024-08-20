#include <sstream>
#include <iomanip>
#include "util/date.hpp"
#include "util/enums/date/months.hpp"

using namespace util;

Date::Date()
{
    this->tp = std::chrono::system_clock::now();
}

Date::Date(std::string dateString, std::string format)
{
    this->tp = this->parseStringDate(dateString, format);
}

Date::Date(std::string dateString)
{
    this->tp = this->parseStringDateddMMYYYYseparetedBySlash(dateString);
}

std::chrono::system_clock::time_point Date::parseStringDate(std::string dateString, std::string format)
{
    std::tm tm = {};
    std::istringstream ss(dateString);
    ss >> std::get_time(&tm, format.c_str());
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

std::chrono::system_clock::time_point Date::parseStringDateddMMYYYYseparetedBySlash(std::string dateString)
{
    return Date::parseStringDate(dateString, "%d/%m/%Y");
}

int Date::getYDay()
{
    time_t tt = std::chrono::system_clock::to_time_t(this->tp);
    tm local = *localtime(&tt);
    return local.tm_yday + 1;
}

int Date::getMDay()
{
    time_t tt = std::chrono::system_clock::to_time_t(this->tp);
    tm local = *localtime(&tt);
    return local.tm_mday;
}

int Date::getMonth()
{
    time_t tt = std::chrono::system_clock::to_time_t(this->tp);
    tm local = *localtime(&tt);
    return local.tm_mon + 1;
}

int Date::getYear()
{
    time_t tt = std::chrono::system_clock::to_time_t(this->tp);
    tm local = *localtime(&tt);
    return local.tm_year + 1900;
}

bool Date::isLeapYear()
{
    if (this->getYear() % 100 == 0)
    {
        return this->getYear() % 400 == 0;
    }
    return this->getYear() % 4 == 0;
}

std::string Date::to_string()
{
    std::string toRet = "";
    int year = this->getYear();
    int month = this->getMonth();
    int monthDay = this->getMDay();
    toRet += std::to_string(monthDay) + "-";
    toRet += util::getMonthName(month) + "-";
    toRet += std::to_string(year);
    return toRet;
}

std::string Date::toDBFormat(){
    std::string toRet = "";
    int year = this->getYear();
    int month = this->getMonth();
    int monthDay = this->getMDay();
    toRet += std::to_string(monthDay) + "-";
    toRet += std::to_string(month) + "-";
    toRet += std::to_string(year);
    return toRet;
}

bool Date::isAfter(Date *other)
{
    return this->diffDaysTo(other) < 0;
}

int Date::diffDaysTo(Date *other) // negative if this is after other
{
    std::chrono::time_point otherTP = other->tp;
    std::chrono::duration durationDiff = other->tp - this->tp;
    auto secondDiff = std::chrono::duration_cast<std::chrono::seconds>(durationDiff);
    return secondDiff.count() / 60 / 60 / 24;
}

int Date::diffYearsTo(Date *other) // negative if this is after other
{
    int yearDiff;
    if (this->isAfter(other))
    {
        yearDiff = this->getYear() - other->getYear();
        if (this->getYDay() < other->getYDay())
        {
            yearDiff -= 1;
        }
        yearDiff = -1 * yearDiff;
    }
    else
    {
        yearDiff = other->getYear() - this->getYear();
        if (other->getYDay() < this->getYDay())
        {
            yearDiff -= 1;
        }
    }
    return yearDiff;
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

time_t Date::getUniqueTimestamp()
{
    const auto p1 = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(p1.time_since_epoch()).count();
}
