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
    this->properties = {};
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
    if (!this->properties[newEntry->getPropertiesDBCode()])
    {
        return -1;
    }
    return this->properties[newEntry->getPropertiesDBCode()]->sellItems((SellingEntry*) newEntry);
}

void InventorySystem::purchaseProperties(Entry *newEntry)
{
    if (!this->properties[newEntry->getPropertiesDBCode()])
    {
        return;
    }
    this->properties[newEntry->getPropertiesDBCode()]->addPurchase((PurchaseEntry*) newEntry);
}

void InventorySystem::addNewItem(Sellable *newSellable)
{
    this->sellables[newSellable->getDBCode()] = newSellable;
}

void InventorySystem::addNewProperty(Depreciable *newDepreciable){
    this->properties[newDepreciable->getDBCode()] = newDepreciable;
}

std::string InventorySystem::to_string()
{
    std::string toRet = "";
    for (auto it = this->sellables.begin() ; it != this->sellables.end() ; it++)
    {
        toRet += it->second->to_string();
    }
    for (auto it = this->properties.begin() ; it != this->properties.end() ; it++){
        toRet += it->second->toString();
    }
    return toRet;
}