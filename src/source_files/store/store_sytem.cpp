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
        cogs += this->iSystem->sellSellables(entry);
    }
    std::string incRevDesc = "Selling for transaction code: " + sellingTransaction->getDBCode();
    std::string incCOGSDesc = "Increase COGS for transaction Code: " + sellingTransaction->getDBCode();
    accounting::Transaction *accountingTransaction = util::factory::GoodsSellingFactory(incRevDesc, sellAmount, sellAmount, 0).createTransaction();
    accounting::Transaction *accountingTransaction2 = util::factory::GoodsSoldCOGSFactory(incCOGSDesc, cogs).createTransaction();
    this->aSystem->addTransaction(accountingTransaction);
    this->aSystem->addTransaction(accountingTransaction2);
}

void StoreSystem::buyItem(PurchaseTransaction *purchaseTransaction)//break here
{   
    double purchaseAmount = 0;
    for (inventory::Entry *entry : purchaseTransaction->getAllEntries())
    {
        purchaseAmount += entry->getPrice() * entry->getQty();
        this->iSystem->purchaseSellables(entry);
    }
    std::string description = "Purchase for transaction code: " + purchaseTransaction->getDBCode();
    accounting::Transaction *accountingTransaction = util::factory::GoodsPurchaseFactory(description, purchaseAmount, purchaseAmount, 0).createTransaction();
    this->aSystem->addTransaction(accountingTransaction);
}

void StoreSystem::buyProperty(PurchaseTransaction *purchaseTransaction){
    double amount = 0.0;
    for (inventory::Entry *entry : purchaseTransaction->getAllEntries()){
        this->iSystem->purchaseProperties(entry);
        amount += entry->getPrice();
    }
    std::string description = "Purchase for transaction code: " + purchaseTransaction->getDBCode();
    accounting::Transaction *accountingTransaction = util::factory::BuyEquipmentFactory(description, amount, amount, 0).createTransaction();
    this->aSystem->addTransaction(accountingTransaction);
}

void StoreSystem::disposeProperty(SellingTransaction *sellingTransaction){//one transaction one property
    inventory::Depreciable *toDispose = NULL;
    double sellAmount = 0.0;
    double propertyValuation = 0.0;
    for (inventory::Entry *entry : sellingTransaction->getAllEntries()){
        sellAmount += entry->getPrice();
        propertyValuation += this->iSystem->sellProperties(entry);
        toDispose = (inventory::Depreciable*) this->iSystem->getProperty(entry->getPropertiesDBCode());//TO DO: verify the casting process
    }
    std::string description = "Selling for transaction code : " + sellingTransaction->getDBCode();
    accounting::Transaction *accountingTransaction = util::factory::SellEquipmentFactory(description, toDispose->getCurrentAccumulatedDepreciation(), propertyValuation, sellAmount, 0).createTransaction();
    this->aSystem->addTransaction(accountingTransaction);
}

void StoreSystem::addItem(inventory::Sellable *newSellable){
    this->iSystem->addNewItem(newSellable);
}

void StoreSystem::addProperty(inventory::Depreciable *newProperty){
    this->iSystem->addNewProperty(newProperty);
}

std::string StoreSystem::toString(){
    std::string toRet = "";
    toRet += this->iSystem->to_string();
    toRet += this->aSystem->to_string();
    return toRet;
}