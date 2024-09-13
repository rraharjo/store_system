#include "store/store_system.hpp"
using namespace store;

StoreSystem::StoreSystem()
{
    this->aSystem = accounting::AccountingSystem::getInstance();
    this->iSystem = inventory::InventorySystem::getInstance();
    this->purchaseTransactions = {};
    this->sellingTransactions = {};
}

void StoreSystem::sellItem(SellingTransaction *sellingTransaction)
{
    double cogs = 0;
    double sellAmount = 0;
    for (inventory::Entry *entry : sellingTransaction->getAllEntries())
    {
        sellAmount += entry->getPrice() * entry->getQty();
        cogs += this->iSystem->sellItem(entry);
    }
    std::string description = "Selling for transaction code: " + std::to_string(sellingTransaction->getDBCode());
    accounting::Transaction *accountingTransaction = util::factory::GoodsSellingFactory(description, sellAmount, sellAmount, 0).createTransaction();
    this->aSystem->addTransaction(accountingTransaction);
}

void StoreSystem::buyItem(PurchaseTransaction *purchaseTransaction)
{   
    double purchaseAmount = 0;
    for (inventory::Entry *entry : purchaseTransaction->getAllEntries())
    {
        purchaseAmount += entry->getPrice() * entry->getQty();
        this->iSystem->purchaseItem(entry);
    }
    std::string description = "Purchase for transaction code: " + std::to_string(purchaseTransaction->getDBCode());
    accounting::Transaction *accountingTransaction = util::factory::GoodsPurchaseFactory(description, purchaseAmount, purchaseAmount, 0).createTransaction();
    this->aSystem->addTransaction(accountingTransaction);
}

/*void StoreSystem::buyProperty(inventory::Depreciable *newProperty){
    this->iSystem->addNewProperty(newProperty);
    std::string description = "Purchase of property of code : " + std::to_string(newProperty->getDBCode());
    accounting::Transaction *accountingTransaction = util::factory::BuyEquipmentFactory(description, newProperty->getPurchaseCost(), newProperty->getPurchaseCost(), 0).createTransaction();
    this->aSystem->addTransaction(accountingTransaction);
}

void StoreSystem::disposeProperty(int dbCode){
    inventory::Depreciable *disposed = this->iSystem->disposeProperty(dbCode);
    if (!disposed){
        return;
    }
    std::string description = "Purchase of property of code : " + std::to_string(disposed->getDBCode());
    accounting::Transaction *accountingTransaction = util::factory::SellEquipmentFactory(description, )//how to know the price?
    this->aSystem->addTransaction(accountingTransaction);
}*/

void StoreSystem::addItem(inventory::Sellable *newSellable){
    this->iSystem->addNewItem(newSellable);
}

std::string StoreSystem::toString(){
    std::string toRet = "";
    toRet += this->iSystem->to_string();
    toRet += this->aSystem->to_string();
    return toRet;
}