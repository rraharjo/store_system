#include "inventory/assets/equipment.hpp"
using namespace inventory;

Equipment::Equipment(std::string db_code,
                     std::string name,
                     std::string item_code,
                     double total_value,
                     double residual_value,
                     int year_useful_life,
                     util::Date *date_bought,
                     util::Date *last_depreciation_date,
                     util::Date *date_sold)
    : Asset(util::enums::PrimaryKeyPrefix::EQUIPMENT,
            db_code,
            name,
            item_code,
            total_value,
            residual_value,
            year_useful_life,
            date_bought,
            last_depreciation_date,
            date_sold)
{
    this->depreciation_method = std::make_unique<util::DoubleDecliningDepreciation>(this->get_total_value(), this->get_year_useful_life());
}

Equipment::Equipment(std::string name, std::string item_code, double residual_value, int year_useful_life, util::Date *date_bought)
    : Equipment("", name, item_code, 0, residual_value, year_useful_life, date_bought, date_bought, NULL)
{
}

Equipment::~Equipment()
{
    #ifdef DEBUG
    std::cout << "Deleting Equipment" << std::endl;
#endif
}

double Equipment::get_reduced_value_at_year(int year)
{
    return this->get_depreciation_expense_at_year(year);
}

double Equipment::get_reduced_value_current_year()
{
    return this->get_current_depreciation_expense();
}

double Equipment::get_depreciation_expense_at_year(int year)
{
    if (year < 0 || year > this->year_useful_life)
    {
        return -1;
    }
    return this->depreciation_method->get_depreciation_expense_at_year(year);
}

double Equipment::get_accumulated_depreciation_at_year(int year)
{
    if (year < 0 || year > this->year_useful_life)
    {
        return -1;
    }
    return this->depreciation_method->get_accumulated_depreciation_at_year(year);
}

double Equipment::get_value_at_year(int year)
{
    if (year < 0 || year > this->year_useful_life)
    {
        return -1;
    }
    return this->get_total_value() - this->get_accumulated_depreciation_at_year(year);
}

// should be accounting ending period
double Equipment::get_current_depreciation_expense()
{
    util::Date *now = new util::Date();
    int age = this->date_bought->diff_years_to(now);
    return this->get_depreciation_expense_at_year(age);
}

double Equipment::get_current_accumulated_depreciation()
{
    util::Date *now = new util::Date();
    int age = this->date_bought->diff_years_to(now);
    return this->get_accumulated_depreciation_at_year(age);
}

double Equipment::get_current_value()
{
    util::Date *now = new util::Date();
    int age = this->date_bought->diff_years_to(now);
    delete now;
    return this->get_value_at_year(age);
}

void Equipment::set_total_value(double new_value)
{
    if (new_value < this->get_total_value())
    {
        return;
    }
    Asset::set_total_value(new_value);
    this->depreciation_method->set_cost(new_value);
}