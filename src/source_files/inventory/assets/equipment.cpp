#include "inventory/assets/equipment.hpp"
using namespace inventory;

std::vector<Asset *> Equipment::generateFromDatabase()
{
    std::vector<Asset *> toRet;
    std::vector<std::vector<std::string>> records = Asset::classTable->getRecords();
    for (std::vector<std::string> &record : records)
    {
        util::Date *purchase = NULL, *sold = NULL;
        if (record[5] != "")
        {
            purchase = new util::Date(record[5], "%Y-%m-%d");
        }
        if (record[6] != "")
        {
            sold = new util::Date(record[6], "%Y-%m-%d");
        }
        Asset *newEquipment = new Equipment(record[0], record[1], "", std::stod(record[2]),
                                                std::stod(record[3]), std::stoi(record[4]), purchase, sold);
        std::vector<PurchaseEntry *> entries = PurchaseEntry::generateFromDatabase(newEquipment->getDBCode());
        for (PurchaseEntry *entry : entries){
            newEquipment->addExistingPurchaseEntry(entry);
        }
        toRet.push_back(newEquipment);
    }
    return toRet;
}

Equipment::Equipment(std::string dbCode, std::string name, std::string itemCode,
                     double totalValue, double residualValue, int yearUsefulLife, util::Date *dateBought, util::Date *dateSold)
    : Asset(dbCode, name, itemCode, totalValue, residualValue, yearUsefulLife, dateBought, dateSold)
{
    this->depreciationMethod = new util::DoubleDecliningDepreciation(this->getTotalValue(), this->getYearUsefulLife());
}

Equipment::Equipment(std::string name, std::string itemCode, double residualValue, int yearUsefulLife, util::Date *dateBought)
    : Equipment("", name, itemCode, 0, residualValue, yearUsefulLife, dateBought, NULL)
{
}

void Equipment::addExistingPurchaseEntry(PurchaseEntry *entry){
    Asset::addExistingPurchaseEntry(entry);
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