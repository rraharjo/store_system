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
    util::Date *transactionDate = new util::Date();
    std::string incRevDesc = "Selling inventory";
    std::string incCOGSDesc = "Increase cost of goods sold";
    accounting::Transaction *accountingTransaction =
        util::factory::GoodsSellingFactory(transactionDate, incRevDesc, sellingTransaction->getDBCode(), sellAmount, sellAmount, 0)
            .createTransaction();
    accounting::Transaction *accountingTransaction2 =
        util::factory::GoodsSoldCOGSFactory(transactionDate, incCOGSDesc, sellingTransaction->getDBCode(), cogs)
            .createTransaction();
    this->aSystem->addTransaction(accountingTransaction);
    this->aSystem->addTransaction(accountingTransaction2);
}

void StoreSystem::buyItem(PurchaseTransaction *purchaseTransaction) // break here
{
    double purchaseAmount = 0;
    for (inventory::Entry *entry : purchaseTransaction->getAllEntries())
    {
        purchaseAmount += entry->getPrice() * entry->getQty();
        this->iSystem->purchaseSellables(entry);
    }
    util::Date *transactionDate = new util::Date();
    std::string description = "Purchase inventory";
    accounting::Transaction *accountingTransaction =
        util::factory::GoodsPurchaseFactory(transactionDate, description, purchaseTransaction->getDBCode(),
                                            purchaseAmount, purchaseAmount, 0)
            .createTransaction();
    this->aSystem->addTransaction(accountingTransaction);
}

void StoreSystem::buyAsset(PurchaseTransaction *purchaseTransaction)
{
    double amount = 0.0;
    for (inventory::Entry *entry : purchaseTransaction->getAllEntries())
    {
        this->iSystem->purchaseProperties(entry);
        amount += entry->getPrice();
    }
    util::Date *transactionDate = new util::Date();
    std::string description = "Purchase asset";
    accounting::Transaction *accountingTransaction =
        util::factory::BuyEquipmentFactory(transactionDate, description, purchaseTransaction->getDBCode(), amount, amount, 0)
            .createTransaction();
    this->aSystem->addTransaction(accountingTransaction);
}

void StoreSystem::disposeAsset(SellingTransaction *sellingTransaction)
{ // one transaction one property
    inventory::Equipment *toDispose = NULL;
    double sellAmount = 0.0;
    double propertyValuation = 0.0;
    for (inventory::Entry *entry : sellingTransaction->getAllEntries())
    {
        sellAmount += entry->getPrice();
        propertyValuation += this->iSystem->sellProperties(entry);
        toDispose = (inventory::Equipment *)this->iSystem->getProperty(entry->getPropertiesDBCode()); // TO DO: verify the casting process
    }
    util::Date *transactionDate = new util::Date();
    std::string description = "Asset disposal";
    accounting::Transaction *accountingTransaction =
        util::factory::SellEquipmentFactory(transactionDate, description, sellingTransaction->getDBCode(),
                                            toDispose->getCurrentAccumulatedDepreciation(), propertyValuation,
                                            sellAmount, 0)
            .createTransaction();
    this->aSystem->addTransaction(accountingTransaction);
}

void StoreSystem::addItem(inventory::Inventory *newSellable)
{
    this->iSystem->addNewItem(newSellable);
}

void StoreSystem::addProperty(inventory::Equipment *newProperty)
{
    this->iSystem->addNewProperty(newProperty);
}

std::string StoreSystem::toString()
{
    std::string toRet = "";
    toRet += this->iSystem->to_string();
    toRet += this->aSystem->to_string();
    return toRet;
}