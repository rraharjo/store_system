#include "inventory/assets/equipment.hpp"
using namespace inventory;
int Equipment::nextItemCode = 0;//TO DO: change it to count(*)

Equipment::Equipment(std::string name, std::string itemCode, double residualValue, int yearUsefulLife, util::Date *dateBought) : 
Asset::Asset(name, itemCode, residualValue, yearUsefulLife, dateBought)
{
    this->depreciationMethod = new util::DoubleDecliningDepreciation(this->getTotalValue(), this->getYearUsefulLife());
    this->setDBCode(this->createDBCode());
    this->insertToDB();
} 

std::string Equipment::createDBCode(){
    char numAsString[6];
    sprintf(numAsString, "%05d", Equipment::nextItemCode++);
    std::string countAsString = numAsString;
    std::string dbCode = "PRP" + countAsString;
    return dbCode;
}

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
    int age = this->dateBought->diffYearsTo(now);//TO DO: change to year
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

void Equipment::setTotalValue(double newValue){
    if (newValue < this->getTotalValue()){
        return;
    }
    Asset::setTotalValue(newValue);
}