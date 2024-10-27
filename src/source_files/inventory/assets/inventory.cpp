#include "inventory/assets/inventory.hpp"

using namespace inventory;

util::Table *Inventory::classTable = util::InventoryTable::getInstance();

void Inventory::insertToDB() {
    this->insertToDBWithTable(Inventory::classTable);
};

void Inventory::updateToDB() {
    this->updateToDBWithTable(Inventory::classTable);
};

Inventory::Inventory(std::string name, std::string itemCode, double sellingPrice) : Item::Item(name, itemCode)
{
    this->sellingPrice = sellingPrice;
    this->qty = 0;
}

std::vector<std::string> Inventory::getInsertParameter(){
    std::vector<std::string> args;
    args.push_back(util::enums::primaryKeyCodesMap[util::enums::PrimaryKeyCodes::INVENTORY]);
    args.push_back(this->getItemCode());
    args.push_back(this->getName());
    args.push_back(std::to_string(this->getSellingPrice()));
    return args;
};

std::vector<std::string> Inventory::getUpdateParameter(){
    std::vector<std::string> args;
    args.push_back(this->getItemCode());
    args.push_back(this->getName());
    args.push_back(std::to_string(this->getSellingPrice()));
    return args;
};

double Inventory::sellItems(SellingEntry *entry)
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

void Inventory::addPurchase(PurchaseEntry *entry)
{
    this->purchaseHistory->addEntry(entry);
    this->qty += entry->getQty();
}

double Inventory::getSellingPrice()
{
    return this->sellingPrice;
}

void Inventory::setSellingPrice(double newPrice)
{
    this->sellingPrice = newPrice;
}

std::string Inventory::to_string()
{
    std::string toRet = "";
    toRet += "DB code: " + this->getDBCode() + "\n";
    toRet += "item code: " + this->itemCode + "\n";
    toRet += "name: " + this->name + "\n";
    toRet += "qty: " + std::to_string(this->qty) + "\n";
    return toRet;
}