#include "inventory/inventory_system.hpp"
using namespace inventory;

InventorySystem *InventorySystem::instance = NULL;

InventorySystem *InventorySystem::getInstance()
{
    if (InventorySystem::instance == NULL)
    {
        // To Do: Test if this is working properly (Nov 1, 2024)
        InventorySystem::instance = new InventorySystem();
        std::vector<Inventory *> inventories = Inventory::generateFromDatabase();
        for (Inventory *inventory : inventories){
            InventorySystem::instance->sellables[inventory->getDBCode()] = inventory;
        }
        std::vector<Asset *> equipments = Equipment::generateFromDatabase();
        for (Asset *equipment : equipments){
            InventorySystem::instance->assets[equipment->getDBCode()] = equipment;
        }
    }
    return InventorySystem::instance;
}

InventorySystem::InventorySystem()
{
    this->sellables = {};
    this->assets = {};
}

void InventorySystem::addExistingInventory(Inventory *inv){
    this->addNewItem(inv);
}

void InventorySystem::addExistingAsset(Asset *asset){
    this->addNewProperty(asset);
}

Asset *InventorySystem::getProperty(std::string dbCode)
{
    return this->assets[dbCode];
}

Inventory *InventorySystem::getInventory(std::string dbCode){
    return this->sellables[dbCode];
}

double InventorySystem::sellSellables(Entry *newEntry)
{
    if (!this->sellables[newEntry->getSellableDBCode()])
    {
        return -1;
    }
    return this->sellables[newEntry->getSellableDBCode()]->sellItems((SellingEntry *)newEntry);
}

void InventorySystem::purchaseSellables(Entry *newEntry)
{
    if (!this->sellables[newEntry->getSellableDBCode()])
    {
        return;
    }
    this->sellables[newEntry->getSellableDBCode()]->addPurchase((PurchaseEntry *)newEntry);
}

double InventorySystem::sellProperties(Entry *newEntry)
{
    if (!this->assets[newEntry->getPropertiesDBCode()])
    {
        return -1;
    }
    return this->assets[newEntry->getPropertiesDBCode()]->sellItems((SellingEntry *)newEntry);
}

void InventorySystem::purchaseProperties(Entry *newEntry)
{
    if (!this->assets[newEntry->getPropertiesDBCode()])
    {
        return;
    }
    this->assets[newEntry->getPropertiesDBCode()]->addPurchase((PurchaseEntry *)newEntry);
}

void InventorySystem::addNewItem(Inventory *newSellable)
{
    if (this->sellables.find(newSellable->getDBCode()) == this->sellables.end())
    {
        this->sellables[newSellable->getDBCode()] = newSellable;
    }
}

void InventorySystem::addNewProperty(Asset *newDepreciable)
{
    if (this->assets.find(newDepreciable->getDBCode()) == this->assets.end())
    {
        this->assets[newDepreciable->getDBCode()] = newDepreciable;
    }
}

std::string InventorySystem::to_string()
{
    std::string toRet = "";
    for (auto it = this->sellables.begin(); it != this->sellables.end(); it++)
    {
        toRet += it->second->to_string();
    }
    for (auto it = this->assets.begin(); it != this->assets.end(); it++)
    {
        toRet += it->second->toString();
    }
    return toRet;
}