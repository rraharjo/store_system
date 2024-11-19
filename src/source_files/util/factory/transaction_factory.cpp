#include "util/factory/transaction_factory.hpp"
using namespace util::factory;

AccountingTransactionFactory::AccountingTransactionFactory(util::Date *transactionDate, std::string transactionName, std::string foreignID)
{
    this->transactionDate = transactionDate;
    this->transactionName = transactionName;
    this->foreignID = foreignID;
}

// Purchasing Goods
accounting::Transaction *GoodsPurchaseFactory::createTransaction()
{
    if (this->purchaseAmount != this->paidCash + this->paidCredit)
    {
        throw std::invalid_argument("total paid amount does not match purchase amount");
    }
    accounting::Transaction *newTransaction = new accounting::Transaction(this->transactionName, this->foreignID);
    newTransaction->insertToDB();
    accounting::Entry *increaseInventory =
        new accounting::Entry(newTransaction->getDBCode(), true, this->purchaseAmount,
                              util::enums::TAccounts::INVENTORY);
    increaseInventory->insertToDB();
    newTransaction->addEntry(increaseInventory);
    if (this->paidCash > 0.0)
    {
        accounting::Entry *reduceCash =
            new accounting::Entry(newTransaction->getDBCode(), false,
                                  this->paidCash, util::enums::TAccounts::CASH);
        reduceCash->insertToDB();
        newTransaction->addEntry(reduceCash);
    }
    if (this->paidCredit > 0.0)
    {
        accounting::Entry *increasePayable =
            new accounting::Entry(newTransaction->getDBCode(), false, this->paidCredit,
                                  util::enums::TAccounts::ACCPAYABLE);
        increasePayable->insertToDB();
        newTransaction->addEntry(increasePayable);
    }
    return newTransaction;
};

GoodsPurchaseFactory::GoodsPurchaseFactory(util::Date *transactionDate, std::string transactionName, std::string foreignID, double purchaseAmount, double paidCash, double paidCredit) : AccountingTransactionFactory(transactionDate, transactionName, foreignID)
{
    this->purchaseAmount = purchaseAmount;
    this->paidCash = paidCash;
    this->paidCredit = paidCredit;
}

// Selling Goods
accounting::Transaction *GoodsSellingFactory::createTransaction()
{
    if (this->sellAmount != this->paidCash + this->paidCredit)
    {
        throw std::invalid_argument("total paid amount does not match purchase amount");
    }
    accounting::Transaction *newTransaction = new accounting::Transaction(this->transactionName, this->foreignID);
    newTransaction->insertToDB();
    accounting::Entry *increaseRevenue =
        new accounting::Entry(newTransaction->getDBCode(), false, this->sellAmount,
                              util::enums::TAccounts::REV);
    increaseRevenue->insertToDB();
    newTransaction->addEntry(increaseRevenue);
    if (this->paidCash > 0.0)
    {
        accounting::Entry *increaseCash =
            new accounting::Entry(newTransaction->getDBCode(), true, this->paidCash,
                                  util::enums::TAccounts::CASH);
        increaseCash->insertToDB();
        newTransaction->addEntry(increaseCash);
    }
    if (this->paidCredit > 0.0)
    {
        accounting::Entry *increaseReceivable =
            new accounting::Entry(newTransaction->getDBCode(), true, this->paidCredit,
                                  util::enums::TAccounts::ACCTRCV);
        increaseReceivable->insertToDB();
        newTransaction->addEntry(increaseReceivable);
    }
    return newTransaction;
}

GoodsSellingFactory::GoodsSellingFactory(util::Date *transactionDate, std::string transactionName, std::string foreignID, double sellAmount, double paidCash, double paidCredit) : AccountingTransactionFactory(transactionDate, transactionName, foreignID)
{
    this->sellAmount = sellAmount;
    this->paidCash = paidCash;
    this->paidCredit = paidCredit;
}

// Adjusting Cost of Goods Sold
accounting::Transaction *GoodsSoldCOGSFactory::createTransaction()
{
    accounting::Transaction *newTransaction = new accounting::Transaction(this->transactionName, this->foreignID);
    newTransaction->insertToDB();
    accounting::Entry *reduceInventory =
        new accounting::Entry(newTransaction->getDBCode(), false, this->cogs,
                              util::enums::TAccounts::INVENTORY);
    reduceInventory->insertToDB();
    newTransaction->addEntry(reduceInventory);
    accounting::Entry *increaseCOGS =
        new accounting::Entry(newTransaction->getDBCode(), true, this->cogs,
                              util::enums::TAccounts::COGS);
    increaseCOGS->insertToDB();
    newTransaction->addEntry(increaseCOGS);
    return newTransaction;
}

GoodsSoldCOGSFactory::GoodsSoldCOGSFactory(util::Date *transactionDate, std::string transactionName,
                                           std::string foreignID, double cogs)
    : AccountingTransactionFactory(transactionDate, transactionName, foreignID)
{
    this->cogs = cogs;
}

// Buy Equipment Transaction
accounting::Transaction *BuyEquipmentFactory::createTransaction()
{
    if (this->equipmentValue != this->paidCash + this->paidCredit)
    {
        throw std::invalid_argument("total paid amount does not match purchase amount");
    }
    accounting::Transaction *newTransaction = new accounting::Transaction(this->transactionName, this->foreignID);
    newTransaction->insertToDB();
    accounting::Entry *increaseAssets =
        new accounting::Entry(newTransaction->getDBCode(), true, this->equipmentValue,
                              util::enums::TAccounts::EQUIPMENT);
    increaseAssets->insertToDB();
    newTransaction->addEntry(increaseAssets);
    if (this->paidCash > 0.0)
    {
        accounting::Entry *reduceCash =
            new accounting::Entry(newTransaction->getDBCode(), false, this->paidCash,
                                  util::enums::TAccounts::CASH);
        reduceCash->insertToDB();
        newTransaction->addEntry(reduceCash);
    }
    if (this->paidCredit)
    {
        accounting::Entry *increasePayable =
            new accounting::Entry(newTransaction->getDBCode(), false, this->paidCredit,
                                  util::enums::TAccounts::ACCPAYABLE);
        increasePayable->insertToDB();
        newTransaction->addEntry(increasePayable);
    }
    return newTransaction;
}

BuyEquipmentFactory::BuyEquipmentFactory(util::Date *transactionDate, std::string transactionName,
                                         std::string foreignID, double equipmentValue, double paidCash, double paidCredit)
    : AccountingTransactionFactory(transactionDate, transactionName, foreignID)
{
    this->equipmentValue = equipmentValue;
    this->paidCash = paidCash;
    this->paidCredit = paidCredit;
}

// Sell Equipment Transaction
accounting::Transaction *SellEquipmentFactory::createTransaction()
{
    accounting::Transaction *newTransaction = new accounting::Transaction(this->transactionName, this->foreignID);
    newTransaction->insertToDB();
    if (this->paidCash > 0.0)
    {
        accounting::Entry *increaseCash =
            new accounting::Entry(newTransaction->getDBCode(), true, this->paidCash,
                                  util::enums::TAccounts::CASH);
        increaseCash->insertToDB();
        newTransaction->addEntry(increaseCash);
    }
    if (this->paidCredit > 0.0)
    {
        accounting::Entry *increaseReceivable =
            new accounting::Entry(newTransaction->getDBCode(), true, this->paidCredit,
                                  util::enums::TAccounts::ACCTRCV);
        increaseReceivable->insertToDB();
        newTransaction->addEntry(increaseReceivable);
    }
    accounting::Entry *reduceEquipment =
        new accounting::Entry(newTransaction->getDBCode(), false, this->initialValue,
                              util::enums::TAccounts::EQUIPMENT);
    reduceEquipment->insertToDB();
    newTransaction->addEntry(reduceEquipment);
    accounting::Entry *reduceAccumulatedDepreciation =
        new accounting::Entry(newTransaction->getDBCode(), true, this->accumulatedDepreciation,
                              util::enums::TAccounts::ACCUMDEPRECIATION);
    reduceAccumulatedDepreciation->insertToDB();
    newTransaction->addEntry(reduceAccumulatedDepreciation);
    double revenue = this->paidCash + this->paidCredit + this->accumulatedDepreciation - this->initialValue;
    if (revenue > 0)
    { // Gain
        accounting::Entry *increaseRevenue =
            new accounting::Entry(newTransaction->getDBCode(), false, revenue,
                                  util::enums::TAccounts::REV);
        increaseRevenue->insertToDB();
        newTransaction->addEntry(increaseRevenue);
    }

    if (revenue < 0)
    { // Loss
        accounting::Entry *reduceRevenue =
            new accounting::Entry(newTransaction->getDBCode(), true, std::abs(revenue),
                                  util::enums::TAccounts::REV);
        reduceRevenue->insertToDB();
        newTransaction->addEntry(reduceRevenue);
    }
    return newTransaction;
}

SellEquipmentFactory::SellEquipmentFactory(util::Date *transactionDate, std::string transactionName, std::string foreignID, double accumulatedDepreciation, double initialValue, double paidCash, double paidCredit) : AccountingTransactionFactory(transactionDate, transactionName, foreignID)
{
    this->accumulatedDepreciation = accumulatedDepreciation;
    this->initialValue = initialValue;
    this->paidCash = paidCash;
    this->paidCredit = paidCredit;
}

// Apply Depreciation Transaction
accounting::Transaction *ApplyDepreciationFactory::createTransaction()
{
    accounting::Transaction *newTransaction = new accounting::Transaction(this->transactionName, this->foreignID);
    newTransaction->insertToDB();
    accounting::Entry *increaseAccumulatedDepreciation =
        new accounting::Entry(newTransaction->getDBCode(), false, this->depreciationAmount,
                              util::enums::TAccounts::ACCUMDEPRECIATION);
    increaseAccumulatedDepreciation->insertToDB();
    newTransaction->addEntry(increaseAccumulatedDepreciation);
    accounting::Entry *reduceEquipment =
        new accounting::Entry(newTransaction->getDBCode(), true, this->depreciationAmount,
                              util::enums::TAccounts::DEPREXP);
    reduceEquipment->insertToDB();
    newTransaction->addEntry(reduceEquipment);
    return newTransaction;
}

ApplyDepreciationFactory::ApplyDepreciationFactory(util::Date *transactionDate, std::string transactionName, std::string foreignID, double depreciationAmount) : AccountingTransactionFactory(transactionDate, transactionName, foreignID)
{
    this->depreciationAmount = depreciationAmount;
}

// Pay Wages Transaction
accounting::Transaction *EmployeeWagesFactory::createTransaction()
{
    accounting::Transaction *newTransaction = new accounting::Transaction(this->transactionName, this->foreignID);
    newTransaction->insertToDB();
    accounting::Entry *reduceCash =
        new accounting::Entry(newTransaction->getDBCode(), false, this->wagesAmount,
                              util::enums::TAccounts::CASH);
    reduceCash->insertToDB();
    newTransaction->addEntry(reduceCash);
    accounting::Entry *increaseExpense =
        new accounting::Entry(newTransaction->getDBCode(), true, this->wagesAmount,
                              util::enums::TAccounts::WAGEEXP);
    increaseExpense->insertToDB();
    newTransaction->addEntry(increaseExpense);
    return newTransaction;
}

EmployeeWagesFactory::EmployeeWagesFactory(util::Date *transactionDate, std::string transactionName, std::string foreignID, double wagesAmount) : AccountingTransactionFactory(transactionDate, transactionName, foreignID)
{
    this->wagesAmount = wagesAmount;
}