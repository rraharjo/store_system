#include "inventory/assets/asset.hpp"
using namespace inventory;

util::Table *Asset::classTable = util::AssetsTable::getInstance();

void Asset::insertToDB()
{
    this->insertToDBWithTable(Asset::classTable);
};

void Asset::updateToDB()
{
    this->updateToDBWithTable(Asset::classTable);
};

Asset::Asset(std::string dbCode, std::string name, std::string itemCode,
             double totalValue, double residualValue, int yearUsefulLife,
             util::Date *dateBought, util::Date *lastDepreciationDate, util::Date *dateSold)
    : Item(name, itemCode)
{
    this->setDBCode(dbCode);
    this->name = name;
    this->value = totalValue;
    this->residualValue = residualValue;
    this->yearUsefulLife = yearUsefulLife;
    this->dateBought = dateBought;
    this->lastDepreciationDate = lastDepreciationDate;
    this->expiryDate = dateSold;
}

Asset::Asset(std::string name, std::string itemCode, double residualValue, int yearUsefulLife, util::Date *dateBought)
    : Asset("", name, itemCode, 0, residualValue, yearUsefulLife, dateBought, NULL, NULL)
{
}

void Asset::addExistingPurchaseEntry(PurchaseEntry *entry)
{
    Item::addExistingPurchaseEntry(entry);
}

std::vector<std::string> Asset::getUpdateParameter()
{
    std::vector<std::string> args;
    args.push_back(this->name);
    args.push_back(std::to_string(this->getTotalValue()));
    args.push_back(std::to_string(this->getResidualValue()));
    args.push_back(std::to_string(this->getYearUsefulLife()));
    args.push_back(this->getDateBought()->toDBFormat());
    args.push_back(this->getLastDepreciationDate() ? this->getLastDepreciationDate()->toDBFormat() : "NULL");
    args.push_back(this->getExpiryDate() ? this->getExpiryDate()->toDBFormat() : "NULL");
    return args;
};

double Asset::sellItems(SellingEntry *entry)
{
    this->sellingHistory->addEntry(entry);
    this->expiryDate = entry->getTransactionDate();
    this->updateToDB();
    return this->getTotalValue();
}

void Asset::addPurchase(PurchaseEntry *entry)
{
    setTotalValue(this->value + entry->getPrice());
    this->purchaseHistory->addEntry(entry);
    this->updateToDB();
}

double Asset::getTotalValue()
{
    return this->value;
}

double Asset::getResidualValue()
{
    return this->residualValue;
}

int Asset::getYearUsefulLife()
{
    return this->yearUsefulLife;
}

util::Date *Asset::getDateBought()
{
    return this->dateBought;
}

util::Date *Asset::getLastDepreciationDate()
{
    return this->lastDepreciationDate;
}

util::Date *Asset::getExpiryDate()
{
    return this->expiryDate;
}

double Asset::getCurrentValue()
{
    return this->getTotalValue();
}

void Asset::setTotalValue(double newValue)
{
    this->value = newValue;
}

void Asset::setLastDepreciationDate(util::Date *depreciationDate)
{
    this->lastDepreciationDate = depreciationDate;
    this->updateToDB();
}

std::string Asset::toString()
{
    std::string toRet = "";
    toRet += "DB code " + this->getDBCode() + "\n";
    toRet += "name : " + this->name + "\n";
    toRet += "valuation: " + std::to_string(this->value) + "\n";
    toRet += "purchase date: " + this->getDateBought()->to_string() + "\n";
    toRet += "sold date: ";
    toRet += this->getExpiryDate() ? this->getExpiryDate()->to_string() : "not sold";
    toRet += "\n";
    return toRet;
}