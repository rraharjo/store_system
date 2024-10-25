#include "util/depreciation.hpp"
using namespace util;

DepreciationMethod::DepreciationMethod(double cost, double residualValue, int yearUsefulLife)
{
    this->cost = cost;
    this->residualValue = residualValue;
    this->yearUsefulLife = yearUsefulLife;
}

void DepreciationMethod::setCost(double cost)
{
    this->cost = cost;
}

// Straight line depreciation
StraightLineDepreciation::StraightLineDepreciation(double cost, double residualValue, int yearUsefulLife) : DepreciationMethod::DepreciationMethod(cost, residualValue, yearUsefulLife) {}

double StraightLineDepreciation::getAccumulatedDepreciationAtYear(int year)
{
    if (year < 0 || year > this->yearUsefulLife)
    {
        return -1;
    }
    return StraightLineDepreciation::getAccumulatedDepreciationAtYear(year) * year;
}

double StraightLineDepreciation::getDepreciationExpenseAtYear(int year)
{
    if (year < 0 || year > this->yearUsefulLife)
    {
        return -1;
    }
    return (this->cost - this->residualValue) / this->yearUsefulLife;
}

// Double declining depreciation
void DoubleDecliningDepreciation::setCost(double cost)
{
    DepreciationMethod::setCost(cost);
    DoubleDecliningDepreciation::initiateAccumulatedDepreciation();
}

DoubleDecliningDepreciation::DoubleDecliningDepreciation(double cost, int yearUsefulLife) : DepreciationMethod::DepreciationMethod(cost, 0, yearUsefulLife)
{
    DoubleDecliningDepreciation::initiateAccumulatedDepreciation();
}

void DoubleDecliningDepreciation::initiateAccumulatedDepreciation()
{
    this->accumulatedDepreciation = {0};
    for (int i = 1; i <= this->yearUsefulLife; i++)
    {
        this->accumulatedDepreciation.push_back(this->accumulatedDepreciation[i - 1] + getDepreciationExpenseAtYear(i));
    }
}

double DoubleDecliningDepreciation::getAccumulatedDepreciationAtYear(int year)
{
    if (year > this->yearUsefulLife || year < 0)
    {
        return -1;
    }
    return this->accumulatedDepreciation[year];
}

double DoubleDecliningDepreciation::getDepreciationExpenseAtYear(int year)
{
    if (year > this->yearUsefulLife || year < 0)
    {
        return -1;
    }
    if (year == 0)
    {
        return 0;
    }
    return (this->cost - this->accumulatedDepreciation[year - 1]) * 2 / this->yearUsefulLife;
}
