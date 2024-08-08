#include "util/enums/date/months.hpp"

using namespace util;

std::map<enums::Month, std::string> enums::monthsMap = {
            {enums::JANUARY, "January"},
            {enums::FEBRUARY, "February"},
            {enums::MARCH, "March"},
            {enums::APRIL, "April"},
            {enums::MAY, "May"},
            {enums::JUNE, "June"},
            {enums::JULY, "July"},
            {enums::AUGUST, "August"},
            {enums::SEPTEMBER, "September"},
            {enums::OCTOBER, "October"},
            {enums::NOVEMBER, "November"},
            {enums::DECEMBER, "December"}};

        
enums::Month enums::allMonths[] = {
    enums::JANUARY, 
    enums::FEBRUARY, 
    enums::MARCH, 
    enums::APRIL, 
    enums::MAY, 
    enums::JUNE, 
    enums::JULY, 
    enums::AUGUST, 
    enums::SEPTEMBER, 
    enums::OCTOBER, 
    enums::NOVEMBER, 
    enums::DECEMBER};

int util::asInt(enums::Month month) { return static_cast<int>(month); }

enums::Month util::toMonth(int month)
{
    return static_cast<enums::Month>(month);
}

std::string util::getMonthName(enums::Month month)
{
    return enums::monthsMap[month];
}

std::string util::getMonthName(int month)
{
    return getMonthName(toMonth(month));
}
