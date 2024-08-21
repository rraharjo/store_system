#include <chrono>
#include "inventory/assets/depreciable.hpp"
#include "util/depreciation.hpp"
#include "util/date.hpp"
using namespace inventory;
Depreciable::Depreciable(std::string name, std::string itemCode, double purchaseCost, double residualValue, int yearUsefulLife, util::Date *dateBought) : Item::Item(name, itemCode)
{
    this->purchaseCost = purchaseCost;
    this->residualValue = residualValue;
    this->yearUsefulLife = yearUsefulLife;
    this->dateBought = dateBought;
    this->depreciationMethod = new util::DoubleDecliningDepreciation(this->purchaseCost, this->yearUsefulLife);
    this->insertToDB();
}

void Depreciable::setTable()
{
    this->table = util::DepreciableTable::getInstance();
}

std::vector<std::string> Depreciable::getInsertParameter()
{
    std::vector<std::string> args;
    args.push_back(std::to_string(this->getPurchaseCost()));
    args.push_back(std::to_string(this->getResidualValue()));
    args.push_back(std::to_string(this->getYearUsefulLife()));
    args.push_back(this->getDateBought()->toDBFormat());
    return args;
};

double Depreciable::getPurchaseCost(){
    return this->purchaseCost;
}

double Depreciable::getResidualValue(){
    return this->residualValue;
}

int Depreciable::getYearUsefulLife(){
    return this->yearUsefulLife;
}

util::Date *Depreciable::getDateBought(){
    return this->dateBought;
}

double Depreciable::getDepreciationExpenseAtYear(int year)
{
    if (year < 0 || year > this->yearUsefulLife)
    {
        return -1;
    }
    return this->depreciationMethod->getDepreciationExpenseAtYear(year);
}

double Depreciable::getAccumulatedDepreciationAtYear(int year)
{
    if (year < 0 || year > this->yearUsefulLife)
    {
        return -1;
    }
    return this->depreciationMethod->getAccumulatedDepreciationAtYear(year);
}

double Depreciable::getValueAtYear(int year)
{
    if (year < 0 || year > this->yearUsefulLife)
    {
        return -1;
    }
    return this->purchaseCost - this->getAccumulatedDepreciationAtYear(year);
}

// should be accounting ending period
double Depreciable::getCurrentDepreciationExpense()
{
    util::Date *now = new util::Date();
    int age = this->dateBought->diffDaysTo(now);
    return this->getDepreciationExpenseAtYear(age);
}

double Depreciable::getCurrentAccumulatedDepreciation()
{
    util::Date *now = new util::Date();
    int age = this->dateBought->diffDaysTo(now);
    return this->getAccumulatedDepreciationAtYear(age);
}

double Depreciable::getCurrentValue()
{
    util::Date *now = new util::Date();
    int age = this->dateBought->diffDaysTo(now);
    return this->getValueAtYear(age);
}