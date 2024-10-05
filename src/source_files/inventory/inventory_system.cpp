#include "inventory/inventory_system.hpp"
using namespace inventory;

InventorySystem *InventorySystem::instance = NULL;

InventorySystem* InventorySystem::getInstance(){
    if (InventorySystem::instance == NULL){
        InventorySystem::instance = new InventorySystem();
    }
    return InventorySystem::instance;
}

InventorySystem::InventorySystem()
{
    this->sellables = {};
    this->assets = {};
}

Asset *InventorySystem::getProperty(std::string dbCode){
    return this->assets[dbCode];
}

double InventorySystem::sellSellables(Entry *newEntry)
{
    if (!this->sellables[newEntry->getSellableDBCode()])
    {
        return -1;
    }
    return this->sellables[newEntry->getSellableDBCode()]->sellItems((SellingEntry*) newEntry);
}

void InventorySystem::purchaseSellables(Entry *newEntry)
{
    if (!this->sellables[newEntry->getSellableDBCode()])
    {
        return;
    }
    this->sellables[newEntry->getSellableDBCode()]->addPurchase((PurchaseEntry*) newEntry);
}

double InventorySystem::sellProperties(Entry *newEntry)
{
    if (!this->assets[newEntry->getPropertiesDBCode()])
    {
        return -1;
    }
    return this->assets[newEntry->getPropertiesDBCode()]->sellItems((SellingEntry*) newEntry);
}

void InventorySystem::purchaseProperties(Entry *newEntry)
{
    if (!this->assets[newEntry->getPropertiesDBCode()])
    {
        return;
    }
    this->assets[newEntry->getPropertiesDBCode()]->addPurchase((PurchaseEntry*) newEntry);
}

void InventorySystem::addNewItem(Sellable *newSellable)
{
    this->sellables[newSellable->getDBCode()] = newSellable;
}

void InventorySystem::addNewProperty(Asset *newDepreciable){
    this->assets[newDepreciable->getDBCode()] = newDepreciable;
}

std::string InventorySystem::to_string()
{
    std::string toRet = "";
    for (auto it = this->sellables.begin() ; it != this->sellables.end() ; it++)
    {
        toRet += it->second->to_string();
    }
    for (auto it = this->assets.begin() ; it != this->assets.end() ; it++){
        toRet += it->second->toString();
    }
    return toRet;
}