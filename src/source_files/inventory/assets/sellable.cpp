#include <deque>
#include "inventory/assets/sellable.hpp"

using namespace inventory;

int Sellable::nextItemCode = 0; //TO DO: change to count(*)

Sellable::Sellable(std::string name, std::string itemCode, double sellingPrice) : Item::Item(name, itemCode)
{
    this->setTable();
    this->sellingPrice = sellingPrice;
    this->qty = 0;
    this->setDBCode(this->createDBCode());
    this->insertToDB();
}

std::string Sellable::createDBCode(){
    char numAsString[6];
    sprintf(numAsString, "%05d", Sellable::nextItemCode++);
    std::string countAsString = numAsString;
    std::string dbCode = "SEL" + countAsString;
    return dbCode;
}

void Sellable::setTable()
{
    this->table = util::SellableTable::getInstance();
}

double Sellable::sellItems(SellingEntry *entry)
{
    if (this->qty < entry->getQty())
    {
        throw std::invalid_argument("Purchasing quantity exceeds available quantity");
        return -1;
    }
    this->sellingHistory->addEntry(entry);
    this->qty -= entry->getQty();
    return this->purchaseHistory->sellItemFirstIn(entry->getQty());
}

void Sellable::addPurchase(PurchaseEntry *entry)
{
    this->purchaseHistory->addEntry(entry);
    this->qty += entry->getQty();
}

std::vector<std::string> Sellable::getInsertParameter(){
    std::vector<std::string> args;
    args.push_back(this->getDBCode());
    args.push_back(this->getItemCode());
    args.push_back(this->getName());
    args.push_back(std::to_string(this->getSellingPrice()));
    return args;
};

double Sellable::getSellingPrice()
{
    return this->sellingPrice;
}

void Sellable::setSellingPrice(double newPrice)
{
    this->sellingPrice = newPrice;
}

std::string Sellable::to_string()
{
    std::string toRet = "";
    toRet += "DB code: " + this->getDBCode() + "\n";
    toRet += "item code: " + this->itemCode + "\n";
    toRet += "name: " + this->name + "\n";
    toRet += "qty: " + std::to_string(this->qty) + "\n";
    return toRet;
}