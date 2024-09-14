#include "inventory/assets/items.hpp"

using namespace inventory;

Item::Item(std::string name = "", std::string itemCode = "")
{
    this->name = name;
    this->itemCode = itemCode;
}

std::string Item::getName(){
    return this->name;
}

std::string Item::getItemCode(){
    return this->itemCode;
}

int Item::getQty(){
    return this->qty;
}

double Item::sellItems(SellingEntry *entry)
{
    if (this->qty < entry->getQty())
    {
        throw std::invalid_argument("Purchasing quantity exceeds available quantity");
        return -1;
    }
    this->sellingHistory->addEntry(entry);
    this->qty -= entry->getQty();
    entry->insertToDB();
    return this->purchaseHistory->sellItemFirstIn(entry->getQty());
}

void Item::addPurchase(PurchaseEntry *entry)
{
    this->purchaseHistory->addEntry(entry);
    this->qty += entry->getQty();
    entry->insertToDB();
}