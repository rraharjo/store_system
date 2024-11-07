#include "inventory/assets/inventory.hpp"

using namespace inventory;

util::Table *Inventory::classTable = util::InventoryTable::getInstance();

std::vector<Inventory *> Inventory::generateFromDatabase()
{
    std::vector<Inventory *> toRet;
    std::vector<std::vector<std::string>> records = Inventory::classTable->getRecords();
    for (std::vector<std::string> &record : records)
    {
        Inventory *newInventory = new Inventory(record[0], record[1], record[2], std::stod(record[3]));
        std::vector<PurchaseEntry *> entries = PurchaseEntry::generateFromDatabase(newInventory->getDBCode());
        for (PurchaseEntry *entry : entries)
        {
            newInventory->addExistingPurchaseEntry(entry);
        }
        toRet.push_back(newInventory);
    }
    return toRet;
}

void Inventory::insertToDB()
{
    this->insertToDBWithTable(Inventory::classTable);
};

void Inventory::updateToDB()
{
    this->updateToDBWithTable(Inventory::classTable);
};

Inventory::Inventory(std::string dbCode, std::string itemCode, std::string name, double sellingPrice)
    : Item::Item(name, itemCode)
{
    this->setDBCode(dbCode);
    this->qty = 0;
    this->sellingPrice = sellingPrice;
}

Inventory::Inventory(std::string itemCode, std::string name, double sellingPrice)
    : Inventory("", itemCode, name, sellingPrice)
{
}

std::vector<std::string> Inventory::getInsertParameter()
{
    std::vector<std::string> args;
    args.push_back(util::enums::primaryKeyCodesMap[util::enums::PrimaryKeyCodes::INVENTORY]);
    args.push_back(this->getItemCode());
    args.push_back(this->getName());
    args.push_back(std::to_string(this->getSellingPrice()));
    return args;
};

std::vector<std::string> Inventory::getUpdateParameter()
{
    std::vector<std::string> args;
    args.push_back(this->getItemCode());
    args.push_back(this->getName());
    args.push_back(std::to_string(this->getSellingPrice()));
    return args;
};

void Inventory::addExistingPurchaseEntry(PurchaseEntry *entry)
{
    Item::addExistingPurchaseEntry(entry);
    this->qty += entry->getAvailableQty();
}

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
    this->qty += entry->getAvailableQty();
}

double Inventory::getSellingPrice()
{
    return this->sellingPrice;
}

void Inventory::setSellingPrice(double newPrice)
{
    this->sellingPrice = newPrice;
    this->updateToDB();
}

std::string Inventory::to_string()
{
    std::string toRet = "";
    toRet += "DB code: " + this->getDBCode() + "\n";
    toRet += "item code: " + this->itemCode + "\n";
    toRet += "name: " + this->name + "\n";
    toRet += "selling price: " + std::to_string(this->sellingPrice) + "\n";
    toRet += "qty: " + std::to_string(this->qty) + "\n";
    return toRet;
}