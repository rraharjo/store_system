#include "inventory/inventory_system.hpp"
using namespace inventory;

InventorySystem *InventorySystem::instance = NULL;

int inventory::binSearch(std::vector<Sellable *> sellables, int dbCode)
{
    int start = 0;
    int end = sellables.size() - 1;
    int mid;
    while (start <= end)
    {
        mid = (start + end) / 2;
        if (sellables[mid]->getDBCode() < dbCode)
        {
            start = mid + 1;
        }
        else if (sellables[mid]->getDBCode() > dbCode)
        {
            end = mid - 1;
        }
        else
        {
            return mid;
        }
    }
    return -1;
};

InventorySystem* InventorySystem::getInstance(){
    if (InventorySystem::instance == NULL){
        InventorySystem::instance = new InventorySystem();
    }
    return InventorySystem::instance;
}

InventorySystem::InventorySystem()
{
    this->sellables = {};
}

double InventorySystem::sellItem(Entry *newEntry)
{
    int index = this->itemExist(newEntry->getSellableDBCode());
    if (index == -1)
    {
        return -1;
    }
    return this->sellables[index]->sellItems(newEntry->getQty());
}

int InventorySystem::itemExist(int dbCode)
{
    int index = inventory::binSearch(this->sellables, dbCode);
    return index;
}

void InventorySystem::purchaseItem(Entry *newEntry)
{
    int index = this->itemExist(newEntry->getSellableDBCode());
    if (index == -1)
    {
        return;
    }
    this->sellables[index]->addPurchase((PurchaseEntry*) newEntry);
}

void InventorySystem::addNewItem(Sellable *newSellable)
{
    this->sellables.push_back(newSellable);
}

std::string InventorySystem::to_string()
{
    std::string toRet = "";
    for (Sellable *sellable : this->sellables)
    {
        toRet += sellable->to_string();
    }
    return toRet;
}