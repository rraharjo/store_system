#include "inventory/assets/asset.hpp"
using namespace inventory;

Asset::Asset(std::string name, std::string itemCode, double residualValue, int yearUsefulLife, util::Date *dateBought) : Item::Item(name, itemCode)
{
    this->setTable();
    this->name = name;
    this->residualValue = residualValue;
    this->yearUsefulLife = yearUsefulLife;
    this->dateBought = dateBought;
    this->expiryDate = NULL;
} 

void Asset::setTable()
{
    this->table = util::DepreciableTable::getInstance();
}

std::vector<std::string> Asset::getInsertParameter()
{
    std::vector<std::string> args;
    args.push_back(this->getDBCode());
    args.push_back(this->name);
    args.push_back(std::to_string(this->getTotalValue()));
    args.push_back(std::to_string(this->getResidualValue()));
    args.push_back(std::to_string(this->getYearUsefulLife()));
    args.push_back(this->getDateBought()->toDBFormat());
    args.push_back(this->getExpiryDate() ? this->getExpiryDate()->toDBFormat() : "NULL");
    return args;
};

double Asset::sellItems(SellingEntry *entry){
    this->sellingHistory->addEntry(entry);
    //TO DO: update to db
    return this->getTotalValue();
}

void Asset::addPurchase(PurchaseEntry *entry){
    this->value += entry->getPrice();
    this->purchaseHistory->addEntry(entry);
    this->updateToDB();
}

double Asset::getTotalValue(){
    return this->value;
}

double Asset::getResidualValue(){
    return this->residualValue;
}

int Asset::getYearUsefulLife(){
    return this->yearUsefulLife;
}

util::Date *Asset::getDateBought(){
    return this->dateBought;
}

util::Date *Asset::getExpiryDate(){
    return this->expiryDate;
}

double Asset::getCurrentValue()
{
    return this->getTotalValue();
}

void Asset::setTotalValue(double newValue){
    this->value = newValue;
    //TO DO: update database
}

std::string Asset::toString(){
    std::string toRet = "";
    toRet += "name : ";
    toRet += this->name + "\n";
    toRet += "purchase date: " + this->getDateBought()->to_string() + "\n";
    toRet += "sold date: ";
    toRet += this->getExpiryDate() ? this->getExpiryDate()->to_string() : "not sold";
    toRet += "\n";
    return toRet;
}