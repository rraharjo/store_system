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
    accounting::Transaction *accountingTransaction = util::factory::GoodsSellingFactory(description, sellAmount, 0, 0).createTransaction();
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
    accounting::Transaction *accountingTransaction = util::factory::GoodsPurchaseFactory(description, purchaseAmount, 0, 0).createTransaction();
}