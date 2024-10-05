#include "inventory/assets/depreciable.hpp"
using namespace inventory;
int Depreciable::nextItemCode = 0;//TO DO: change it to count(*)

Depreciable::Depreciable(std::string name, std::string itemCode, double residualValue, int yearUsefulLife, util::Date *dateBought) : 
Asset::Asset(name, itemCode, residualValue, yearUsefulLife, dateBought)
{
    this->depreciationMethod = new util::DoubleDecliningDepreciation(this->getTotalValue(), this->getYearUsefulLife());
    this->setDBCode(this->createDBCode());
    this->insertToDB();
} 

std::string Depreciable::createDBCode(){
    char numAsString[6];
    sprintf(numAsString, "%05d", Depreciable::nextItemCode++);
    std::string countAsString = numAsString;
    std::string dbCode = "PRP" + countAsString;
    return dbCode;
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
    return this->getTotalValue() - this->getAccumulatedDepreciationAtYear(year);
}

// should be accounting ending period
double Depreciable::getCurrentDepreciationExpense()
{
    util::Date *now = new util::Date();
    int age = this->dateBought->diffYearsTo(now);//TO DO: change to year
    return this->getDepreciationExpenseAtYear(age);
}

double Depreciable::getCurrentAccumulatedDepreciation()
{
    util::Date *now = new util::Date();
    int age = this->dateBought->diffYearsTo(now);
    return this->getAccumulatedDepreciationAtYear(age);
}

double Depreciable::getCurrentValue()
{
    util::Date *now = new util::Date();
    int age = this->dateBought->diffYearsTo(now);
    delete now;
    return this->getValueAtYear(age);
}

void Depreciable::setTotalValue(double newValue){
    if (newValue < this->getTotalValue()){
        return;
    }
    Asset::setTotalValue(newValue);
}