#include <deque>
#include "inventory/assets/sellable.hpp"

using namespace inventory;

Sellable::Sellable(std::string name, std::string itemCode, double sellingPrice) : Item::Item(name, itemCode)
{
    this->setTable();
    this->sellingPrice = sellingPrice;
    this->qty = 0;
    this->purchaseHistory = new PurchaseHistory();
    this->sellingHistory = new SellingHistory();
    this->insertToDB();
}

void Sellable::setTable()
{
    this->table = util::SellableTable::getInstance();
}

std::vector<std::string> Sellable::getInsertParameter(){
    std::vector<std::string> args;
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

double Sellable::sellItems(int qty)
{
    if (this->qty < qty)
    {
        throw std::invalid_argument("Purchasing quantity exceeds available quantity");
        return -1;
    }
    this->qty -= qty;
    return this->purchaseHistory->sellItemFirstIn(qty);
}

void Sellable::addPurchase(PurchaseEntry *entry)
{
    this->purchaseHistory->addEntry(entry);
    this->qty += entry->getQty();
    entry->insertToDB();
}

std::string Sellable::to_string()
{
    std::string toRet = "";
    toRet += "DB code: " + std::to_string(this->getDBCode()) + "\n";
    toRet += "item code: " + this->itemCode + "\n";
    toRet += "name: " + this->name + "\n";
    toRet += "qty: " + std::to_string(this->qty) + "\n";
    return toRet;
}