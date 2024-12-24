#include "util/depreciation.hpp"
using namespace util;

DepreciationMethod::DepreciationMethod(double cost, double residual_value, int year_useful_life)
{
    this->cost = cost;
    this->residual_value = residual_value;
    this->year_useful_life = year_useful_life;
}

void DepreciationMethod::set_cost(double cost)
{
    this->cost = cost;
}

// Straight line depreciation
StraightLineDepreciation::StraightLineDepreciation(double cost, double residual_value, int year_useful_life) : DepreciationMethod::DepreciationMethod(cost, residual_value, year_useful_life) {}

double StraightLineDepreciation::get_accumulated_depreciation_at_year(int year)
{
    if (year < 0 || year > this->year_useful_life)
    {
        return -1;
    }
    return StraightLineDepreciation::get_accumulated_depreciation_at_year(year) * year;
}

double StraightLineDepreciation::get_depreciation_expense_at_year(int year)
{
    if (year < 0 || year > this->year_useful_life)
    {
        return -1;
    }
    return (this->cost - this->residual_value) / this->year_useful_life;
}

// Double declining depreciation
void DoubleDecliningDepreciation::set_cost(double cost)
{
    DepreciationMethod::set_cost(cost);
    DoubleDecliningDepreciation::initiate_accumulated_depreciation();
}

DoubleDecliningDepreciation::DoubleDecliningDepreciation(double cost, int year_useful_life) : DepreciationMethod::DepreciationMethod(cost, 0, year_useful_life)
{
    DoubleDecliningDepreciation::initiate_accumulated_depreciation();
}

void DoubleDecliningDepreciation::initiate_accumulated_depreciation()
{
    this->accumulated_depreciation = {0};
    for (int i = 1; i <= this->year_useful_life; i++)
    {
        this->accumulated_depreciation.push_back(this->accumulated_depreciation[i - 1] + get_depreciation_expense_at_year(i));
    }
}

double DoubleDecliningDepreciation::get_accumulated_depreciation_at_year(int year)
{
    if (year > this->year_useful_life || year < 0)
    {
        return -1;
    }
    return this->accumulated_depreciation[year];
}

double DoubleDecliningDepreciation::get_depreciation_expense_at_year(int year)
{
    if (year > this->year_useful_life || year < 0)
    {
        return -1;
    }
    if (year == 0)
    {
        return 0;
    }
    return (this->cost - this->accumulated_depreciation[year - 1]) * 2 / this->year_useful_life;
}
