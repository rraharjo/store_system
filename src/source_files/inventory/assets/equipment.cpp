#include "inventory/assets/equipment.hpp"
using namespace inventory;

Equipment::Equipment(std::string name, std::string itemCode, double residualValue, int yearUsefulLife, util::Date *dateBought)
    : Asset::Asset(name, itemCode, residualValue, yearUsefulLife, dateBought)
{
    this->depreciationMethod = new util::DoubleDecliningDepreciation(this->getTotalValue(), this->getYearUsefulLife());
}

std::vector<std::string> Equipment::getInsertParameter()
{
    std::vector<std::string> args;
    args.push_back(util::enums::primaryKeyCodesMap[util::enums::PrimaryKeyCodes::EQUIPMENT]);
    args.push_back(this->getName());
    args.push_back(std::to_string(this->getTotalValue()));
    args.push_back(std::to_string(this->getResidualValue()));
    args.push_back(std::to_string(this->getYearUsefulLife()));
    args.push_back(this->getDateBought()->toDBFormat());
    args.push_back(this->getExpiryDate() ? this->getExpiryDate()->toDBFormat() : "NULL");
    return args;
};

double Equipment::getDepreciationExpenseAtYear(int year)
{
    if (year < 0 || year > this->yearUsefulLife)
    {
        return -1;
    }
    return this->depreciationMethod->getDepreciationExpenseAtYear(year);
}

double Equipment::getAccumulatedDepreciationAtYear(int year)
{
    if (year < 0 || year > this->yearUsefulLife)
    {
        return -1;
    }
    return this->depreciationMethod->getAccumulatedDepreciationAtYear(year);
}

double Equipment::getValueAtYear(int year)
{
    if (year < 0 || year > this->yearUsefulLife)
    {
        return -1;
    }
    return this->getTotalValue() - this->getAccumulatedDepreciationAtYear(year);
}

// should be accounting ending period
double Equipment::getCurrentDepreciationExpense()
{
    util::Date *now = new util::Date();
    int age = this->dateBought->diffYearsTo(now); // TO DO: change to year
    return this->getDepreciationExpenseAtYear(age);
}

double Equipment::getCurrentAccumulatedDepreciation()
{
    util::Date *now = new util::Date();
    int age = this->dateBought->diffYearsTo(now);
    return this->getAccumulatedDepreciationAtYear(age);
}

double Equipment::getCurrentValue()
{
    util::Date *now = new util::Date();
    int age = this->dateBought->diffYearsTo(now);
    delete now;
    return this->getValueAtYear(age);
}

void Equipment::setTotalValue(double newValue)
{
    if (newValue < this->getTotalValue())
    {
        return;
    }
    Asset::setTotalValue(newValue);
    this->depreciationMethod->setCost(newValue);
}