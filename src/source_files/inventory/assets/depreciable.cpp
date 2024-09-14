#include "inventory/assets/depreciable.hpp"
using namespace inventory;
int Depreciable::nextItemCode = 0;//TO DO: change it to count(*)

Depreciable::Depreciable(std::string name, std::string itemCode, double residualValue, int yearUsefulLife, util::Date *dateBought) : Item::Item(name, itemCode)
{
    this->setTable();
    this->name = name;
    this->residualValue = residualValue;
    this->yearUsefulLife = yearUsefulLife;
    this->dateBought = dateBought;
    this->dateSold = NULL;
    this->depreciationMethod = new util::DoubleDecliningDepreciation(this->purchaseCost, this->yearUsefulLife);
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

void Depreciable::setTable()
{
    this->table = util::DepreciableTable::getInstance();
}

std::vector<std::string> Depreciable::getInsertParameter()
{
    std::vector<std::string> args;
    args.push_back(this->getDBCode());
    args.push_back(this->name);
    args.push_back(std::to_string(this->getPurchaseCost()));
    args.push_back(std::to_string(this->getResidualValue()));
    args.push_back(std::to_string(this->getYearUsefulLife()));
    args.push_back(this->getDateBought()->toDBFormat());
    args.push_back(this->getDateSold() ? this->getDateSold()->toDBFormat() : "NULL");
    return args;
};

double Depreciable::sellItems(SellingEntry *entry){
    this->sellingHistory->addEntry(entry);
    entry->insertToDB();
    return this->purchaseCost;
}

void Depreciable::addPurchase(PurchaseEntry *entry){
    this->purchaseCost += entry->getPrice();
    this->purchaseHistory->addEntry(entry);
    this->updateToDB();
    entry->insertToDB();//TO DO: insert to DB should happen inside purchasehistory
}

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

util::Date *Depreciable::getDateSold(){
    return this->dateSold;
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
    int age = this->dateBought->diffDaysTo(now);//TO DO: change to year
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

std::string Depreciable::toString(){
    std::string toRet = "";
    toRet += "name : ";
    toRet += this->name + "\n";
    toRet += "purchase date: " + this->getDateBought()->to_string() + "\n";
    toRet += "sold date: ";
    toRet += this->getDateSold() ? this->getDateSold()->to_string() : "not sold";
    toRet += "\n";
    return toRet;
}