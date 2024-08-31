#include "util/factory/transaction_factory.hpp"
using namespace util::factory;

AccountingTransactionFactory::AccountingTransactionFactory(util::Date *transactionDate, std::string transactionName)
{
    this->transactionDate = transactionDate;
    this->transactionName = transactionName;
}

AccountingTransactionFactory::~AccountingTransactionFactory()
{
    delete this->transactionDate;
}

// Purchasing Goods
accounting::Transaction *GoodsPurchaseFactory::createTransaction()
{
    if (this->purchaseAmount != this->paidCash + this->paidCredit)
    {
        throw std::invalid_argument("total paid amount does not match purchase amount");
    }
    accounting::Transaction *newTransaction = new accounting::Transaction(this->transactionName);
    accounting::Entry *increaseInventory = new accounting::Entry(true, this->purchaseAmount, util::enums::AccountTitles::ASSETS, util::enums::TAccounts::INVENTORY);
    newTransaction->addEntry(increaseInventory);
    if (this->paidCash > 0.0)
    {
        accounting::Entry *reduceCash = new accounting::Entry(false, this->paidCash, util::enums::AccountTitles::ASSETS, util::enums::TAccounts::CASH);
        newTransaction->addEntry(reduceCash);
    }
    if (this->paidCredit > 0.0)
    {
        accounting::Entry *increasePayable = new accounting::Entry(false, this->paidCredit, util::enums::AccountTitles::LIABILITIES, util::enums::TAccounts::NOTEPAYABLE);
        newTransaction->addEntry(increasePayable);
    }
    return newTransaction;
};

GoodsPurchaseFactory::GoodsPurchaseFactory(util::Date *transactionDate, std::string transactionName, double purchaseAmount, double paidCash, double paidCredit) : AccountingTransactionFactory(transactionDate, transactionName)
{
    this->purchaseAmount = purchaseAmount;
    this->paidCash = paidCash;
    this->paidCredit = paidCredit;
}

GoodsPurchaseFactory::GoodsPurchaseFactory(std::string transactionName, double purchaseAmount, double paidCash, double paidCredit) : GoodsPurchaseFactory(new util::Date(), transactionName, purchaseAmount, paidCash, paidCredit)
{
}

// Selling Goods
accounting::Transaction *GoodsSellingFactory::createTransaction()
{
    if (this->sellAmount != this->paidCash + this->paidCredit)
    {
        throw std::invalid_argument("total paid amount does not match purchase amount");
    }
    accounting::Transaction *newTransaction = new accounting::Transaction(this->transactionName);
    accounting::Entry *increaseRevenue = new accounting::Entry(false, this->sellAmount, util::enums::AccountTitles::STOCKHOLDERSEQUITY, util::enums::TAccounts::REV);
    newTransaction->addEntry(increaseRevenue);
    if (this->paidCash > 0.0)
    {
        accounting::Entry *increaseCash = new accounting::Entry(true, this->paidCash, util::enums::AccountTitles::ASSETS, util::enums::TAccounts::CASH);
        newTransaction->addEntry(increaseCash);
    }
    if (this->paidCredit > 0.0)
    {
        accounting::Entry *increaseReceivable = new accounting::Entry(true, this->paidCredit, util::enums::AccountTitles::ASSETS, util::enums::TAccounts::NOTERCV);
        newTransaction->addEntry(increaseReceivable);
    }
    return newTransaction;
}

GoodsSellingFactory::GoodsSellingFactory(util::Date *transactionDate, std::string transactionName, double sellAmount, double paidCash, double paidCredit) : AccountingTransactionFactory(transactionDate, transactionName)
{
    this->sellAmount = sellAmount;
    this->paidCash = paidCash;
    this->paidCredit = paidCredit;
}

GoodsSellingFactory::GoodsSellingFactory(std::string transactionName, double sellAmount, double paidCash, double paidCredit) : GoodsSellingFactory(new util::Date(), transactionName, sellAmount, paidCash, paidCredit)
{
}

// Adjusting Cost of Goods Sold
accounting::Transaction *GoodsSoldCOGSFactory::createTransaction()
{
    accounting::Transaction *newTransaction = new accounting::Transaction(this->transactionName);
    accounting::Entry *reduceInventory = new accounting::Entry(false, this->cogs, util::enums::AccountTitles::ASSETS, util::enums::TAccounts::INVENTORY);
    newTransaction->addEntry(reduceInventory);
    accounting::Entry *increaseCOGS = new accounting::Entry(true, this->cogs, util::enums::AccountTitles::STOCKHOLDERSEQUITY, util::enums::TAccounts::COGS);
    newTransaction->addEntry(increaseCOGS);
    return newTransaction;
}

GoodsSoldCOGSFactory::GoodsSoldCOGSFactory(util::Date *transactionDate, std::string transactionName, double cogs) : AccountingTransactionFactory(transactionDate, transactionName)
{
    this->cogs = cogs;
}

GoodsSoldCOGSFactory::GoodsSoldCOGSFactory(std::string transactionName, double cogs) : GoodsSoldCOGSFactory(new util::Date(), transactionName, cogs)
{
}

// Buy Equipment Transaction
accounting::Transaction *BuyEquipmentFactory::createTransaction()
{
    if (this->equipmentValue != this->paidCash + this->paidCredit)
    {
        throw std::invalid_argument("total paid amount does not match purchase amount");
    }
    accounting::Transaction *newTransaction = new accounting::Transaction(this->transactionName);
    accounting::Entry *increaseAssets = new accounting::Entry(true, this->equipmentValue, util::enums::AccountTitles::ASSETS, util::enums::TAccounts::EQUIPMENT);
    newTransaction->addEntry(increaseAssets);
    if (this->paidCash > 0.0)
    {
        accounting::Entry *reduceCash = new accounting::Entry(false, this->paidCash, util::enums::AccountTitles::ASSETS, util::enums::TAccounts::CASH);
        newTransaction->addEntry(reduceCash);
    }
    if (this->paidCredit)
    {
        accounting::Entry *increasePayable = new accounting::Entry(false, this->paidCredit, util::enums::AccountTitles::LIABILITIES, util::enums::TAccounts::NOTEPAYABLE);
        newTransaction->addEntry(increasePayable);
    }
    return newTransaction;
}

BuyEquipmentFactory::BuyEquipmentFactory(util::Date *transactionDate, std::string transactionName, double equipmentValue, double paidCash, double paidCredit) : AccountingTransactionFactory(transactionDate, transactionName)
{
    this->equipmentValue = equipmentValue;
    this->paidCash = paidCash;
    this->paidCredit = paidCredit;
}

BuyEquipmentFactory::BuyEquipmentFactory(std::string transactionName, double equipmentValue, double paidCash, double paidCredit) : BuyEquipmentFactory(new util::Date(), transactionName, equipmentValue, paidCash, paidCredit)
{
}

// Sell Equipment Transaction
accounting::Transaction *SellEquipmentFactory::createTransaction()
{
    accounting::Transaction *newTransaction = new accounting::Transaction(this->transactionName);
    if (this->paidCash > 0.0)
    {
        accounting::Entry *increaseCash = new accounting::Entry(true, this->paidCash, util::enums::AccountTitles::ASSETS, util::enums::TAccounts::CASH);
        newTransaction->addEntry(increaseCash);
    }
    if (this->paidCredit)
    {
        accounting::Entry *increaseReceivable = new accounting::Entry(true, this->paidCredit, util::enums::AccountTitles::ASSETS, util::enums::TAccounts::NOTERCV);
        newTransaction->addEntry(increaseReceivable);
    }
    accounting::Entry *reduceEquipment = new accounting::Entry(false, this->initialValue, util::enums::AccountTitles::ASSETS, util::enums::TAccounts::EQUIPMENT);
    newTransaction->addEntry(reduceEquipment);
    accounting::Entry *reduceAccumulatedDepreciation = new accounting::Entry(true, this->accumulatedDepreciation, util::enums::AccountTitles::ASSETS, util::enums::TAccounts::ACCUMDEPRECIATION);
    newTransaction->addEntry(reduceAccumulatedDepreciation);
    double revenue = this->paidCash + this->paidCredit + this->accumulatedDepreciation - this->initialValue;
    if (revenue > 0)
    { // Gain
        accounting::Entry *increaseRevenue = new accounting::Entry(false, revenue, util::enums::AccountTitles::STOCKHOLDERSEQUITY, util::enums::TAccounts::REV);
        newTransaction->addEntry(increaseRevenue);
    }

    if (revenue < 0)
    { // Loss
        accounting::Entry *reduceRevenue = new accounting::Entry(true, std::abs(revenue), util::enums::AccountTitles::STOCKHOLDERSEQUITY, util::enums::TAccounts::REV);
        newTransaction->addEntry(reduceRevenue);
    }
    return newTransaction;
}

SellEquipmentFactory::SellEquipmentFactory(util::Date *transactionDate, std::string transactionName, double accumulatedDepreciation, double initialValue, double paidCash, double paidCredit) : AccountingTransactionFactory(transactionDate, transactionName)
{
    this->accumulatedDepreciation = accumulatedDepreciation;
    this->initialValue = initialValue;
    this->paidCash = paidCash;
    this->paidCredit = paidCredit;
}

SellEquipmentFactory::SellEquipmentFactory(std::string transactionName, double accumulatedDepreciation, double initialValue, double paidCash, double paidCredit) : SellEquipmentFactory(new util::Date(), transactionName, accumulatedDepreciation, initialValue, paidCash, paidCredit)
{
}

// Apply Depreciation Transaction
accounting::Transaction *ApplyDepreciationFactory::createTransaction()
{
    accounting::Transaction *newTransaction = new accounting::Transaction(this->transactionName);
    accounting::Entry *increaseAccumulatedDepreciation = new accounting::Entry(false, this->depreciationAmount, util::enums::AccountTitles::ASSETS, util::enums::TAccounts::ACCUMDEPRECIATION);
    newTransaction->addEntry(increaseAccumulatedDepreciation);
    accounting::Entry *reduceEquipment = new accounting::Entry(true, this->depreciationAmount, util::enums::AccountTitles::STOCKHOLDERSEQUITY, util::enums::TAccounts::DEPREXP);
    newTransaction->addEntry(reduceEquipment);
    return newTransaction;
}

ApplyDepreciationFactory::ApplyDepreciationFactory(util::Date *transactionDate, std::string transactionName, double depreciationAmount) : AccountingTransactionFactory(transactionDate, transactionName)
{
    this->depreciationAmount = depreciationAmount;
}

ApplyDepreciationFactory::ApplyDepreciationFactory(std::string transactionName, double depreciationAmount) : ApplyDepreciationFactory(new util::Date(), transactionName, depreciationAmount)
{
}

// Pay Wages Transaction
accounting::Transaction *EmployeeWagesFactory::createTransaction()
{
    accounting::Transaction *newTransaction = new accounting::Transaction(this->transactionName);
    accounting::Entry *reduceCash = new accounting::Entry(false, this->wagesAmount, util::enums::AccountTitles::ASSETS, util::enums::TAccounts::CASH);
    newTransaction->addEntry(reduceCash);
    accounting::Entry *increaseExpense = new accounting::Entry(true, this->wagesAmount, util::enums::AccountTitles::STOCKHOLDERSEQUITY, util::enums::TAccounts::WAGEEXP);
    newTransaction->addEntry(increaseExpense);
    return newTransaction;
}

EmployeeWagesFactory::EmployeeWagesFactory(util::Date *transactionDate, std::string transactionName, double wagesAmount) : AccountingTransactionFactory(transactionDate, transactionName)
{
    this->wagesAmount = wagesAmount;
}

EmployeeWagesFactory::EmployeeWagesFactory(std::string transactionName, double wagesAmount) : EmployeeWagesFactory(new util::Date(), transactionName, wagesAmount)
{
}