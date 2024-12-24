#include "util/enums/date/months.hpp"

using namespace util;

std::map<enums::Month, std::string> enums::months_map = {
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

        
enums::Month enums::all_months[] = {
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

int util::as_int(enums::Month month) { return static_cast<int>(month); }

enums::Month util::to_month(int month)
{
    return static_cast<enums::Month>(month);
}

std::string util::get_month_name(enums::Month month)
{
    return enums::months_map[month];
}

std::string util::get_month_name(int month)
{
    return get_month_name(to_month(month));
}
