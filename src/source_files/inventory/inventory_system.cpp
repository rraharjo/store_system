#include "inventory/inventory_system.hpp"
using namespace inventory;

InventorySystem *InventorySystem::instance = NULL;

InventorySystem *InventorySystem::getInstance()
{
    if (InventorySystem::instance == NULL)
    {
        InventorySystem::instance = new InventorySystem();
        std::vector<Inventory *> inventories = Inventory::generateFromDatabase();
        for (Inventory *inventory : inventories)
        {
            InventorySystem::instance->sellables[inventory->getDBCode()] = inventory;
        }
        std::vector<Asset *> equipments = Equipment::generateFromDatabase();
        for (Asset *equipment : equipments)
        {
            InventorySystem::instance->assets[equipment->getDBCode()] = equipment;
        }
        InventorySystem::instance->setASystem(accounting::AccountingSystem::getInstance());
    }
    return InventorySystem::instance;
}

InventorySystem::InventorySystem()
{
    this->sellables = {};
    this->assets = {};
}

void InventorySystem::setASystem(accounting::AccountingSystem *aSystem)
{
    this->aSystem = aSystem;
}

void InventorySystem::addExistingInventory(Inventory *inv)
{
    this->addNewItem(inv);
}

void InventorySystem::addExistingAsset(Asset *asset)
{
    this->addNewProperty(asset);
}

Asset *InventorySystem::getProperty(std::string dbCode)
{
    return this->assets[dbCode];
}

Inventory *InventorySystem::getInventory(std::string dbCode)
{
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

void InventorySystem::applyDepreciation(std::string assetDBCode)
{
    Asset *asset = this->assets[assetDBCode];
    util::Date *now = new util::Date();
    if ((!asset->getLastDepreciationDate() && now->diff_years_to(asset->getDateBought()) >= 0) ||
        now->diff_years_to(asset->getLastDepreciationDate()) >= 0)
    {
        delete now;
        return;
    }
    double depreciationAmountThisYear = asset->getReducedValueCurrentYear();
    util::Date *newDepreciationDate = new util::Date();
    delete asset->getLastDepreciationDate();
    asset->setLastDepreciationDate(newDepreciationDate);
    std::string accountingTransactionTitle = "Incurred depreciation expense";
    accounting::Transaction *newTransaction =
        util::factory::ApplyDepreciationFactory(now, accountingTransactionTitle, assetDBCode, depreciationAmountThisYear)
            .createTransaction();
    this->aSystem->addTransaction(newTransaction);
}

void InventorySystem::applyAllDepreciation()
{
    for (auto it = this->assets.begin(); it != this->assets.end(); it++)
    {
        this->applyDepreciation(it->first);
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