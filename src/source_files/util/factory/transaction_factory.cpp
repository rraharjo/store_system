#include "util/factory/transaction_factory.hpp"
using namespace util::factory;

AccountingTransactionFactory::AccountingTransactionFactory(util::Date* transactionDate, std::string transactionName){
    this->transactionDate = transactionDate;
    this->transactionName = transactionName;
}

AccountingTransactionFactory::~AccountingTransactionFactory(){
    delete this->transactionDate;
}

//Purchasing Goods
accounting::Transaction *GoodsPurchaseFactory::createTransaction() {
    if (this->purchaseAmount != this->paidCash + this->paidCredit){
        throw std::invalid_argument("total paid amount does not match purchase amount");
    }
    accounting::Transaction *newTransaction = new accounting::Transaction(this->transactionName);
    accounting::Entry *increaseInventory = new accounting::Entry(true, this->purchaseAmount, util::enums::AccountTitles::ASSETS, util::enums::TAccounts::INVENTORY);
    newTransaction->addEntry(increaseInventory);
    if (this->paidCash > 0.0){
        accounting::Entry *reduceCash = new accounting::Entry(false, this->paidCash, util::enums::AccountTitles::ASSETS, util::enums::TAccounts::CASH);
        newTransaction->addEntry(reduceCash);
    }
    if (this->paidCredit > 0.0){
        accounting::Entry *increasePayable = new accounting::Entry(false, this->paidCredit, util::enums::AccountTitles::LIABILITIES, util::enums::TAccounts::NOTEPAYABLE);
        newTransaction->addEntry(increasePayable);
    }
    return newTransaction;
};

GoodsPurchaseFactory::GoodsPurchaseFactory(util::Date* transactionDate, double purchaseAmount, double paidCash, double paidCredit, std::string transactionName) : AccountingTransactionFactory(transactionDate, transactionName)
{
    this->purchaseAmount = purchaseAmount;
    this->paidCash = paidCash;
    this->paidCash = paidCredit;
}

GoodsPurchaseFactory::GoodsPurchaseFactory(double purchaseAmount, double paidCash, double paidCredit, std::string transactionName) : GoodsPurchaseFactory(new util::Date(), purchaseAmount, paidCash, paidCredit, transactionName)
{
    
}

//Selling Goods
accounting::Transaction *GoodsSellingFactory::createTransaction()
{
    if (this->sellAmount != this->paidCash + this->paidCredit){
        throw std::invalid_argument("total paid amount does not match purchase amount");
    }
    accounting::Transaction *newTransaction = new accounting::Transaction(this->transactionName);
    accounting::Entry *increaseRevenue = new accounting::Entry(false, this->sellAmount, util::enums::AccountTitles::STOCKHOLDERSEQUITY, util::enums::TAccounts::REV);
    newTransaction->addEntry(increaseRevenue);
    if (this->paidCash > 0.0){
        accounting::Entry *increaseCash = new accounting::Entry(true, this->paidCash, util::enums::AccountTitles::ASSETS, util::enums::TAccounts::CASH);
        newTransaction->addEntry(increaseCash);
    }
    if (this->paidCredit > 0.0){
        accounting::Entry *increaseReceivable = new accounting::Entry(true, this->paidCredit, util::enums::AccountTitles::ASSETS, util::enums::TAccounts::NOTERCV);
        newTransaction->addEntry(increaseReceivable);
    }
    return newTransaction;
}

GoodsSellingFactory::GoodsSellingFactory(util::Date *transactionDate, double sellAmount, double paidCash, double paidCredit, std::string transactionName) : AccountingTransactionFactory(transactionDate, transactionName)
{
    this->sellAmount = sellAmount;
    this->paidCash = paidCash;
    this->paidCredit = paidCredit;
}

GoodsSellingFactory::GoodsSellingFactory(double sellAmount, double paidCash, double paidCredit, std::string transactionName) : GoodsSellingFactory(new util::Date(), sellAmount, paidCash, paidCredit, transactionName)
{
}

//Adjusting Cost of Goods Sold
accounting::Transaction *GoodsSoldCOGS::createTransaction()
{
    accounting::Transaction *newTransaction = new accounting::Transaction(this->transactionName);
    accounting::Entry* reduceInventory = new accounting::Entry(false, this->cogs, util::enums::AccountTitles::ASSETS, util::enums::TAccounts::INVENTORY);
    newTransaction->addEntry(reduceInventory);
    accounting::Entry* increaseCOGS = new accounting::Entry(true, this->cogs, util::enums::AccountTitles::STOCKHOLDERSEQUITY, util::enums::TAccounts::COGS);
    newTransaction->addEntry(increaseCOGS);
    return newTransaction;
}

GoodsSoldCOGS::GoodsSoldCOGS(util::Date* transactionDate, double cogs, std::string transactionName): AccountingTransactionFactory(transactionDate, transactionName)
{
    this->cogs = cogs;
}

GoodsSoldCOGS::GoodsSoldCOGS(double cogs, std::string transactionName) : GoodsSoldCOGS(new util::Date(), cogs, transactionName)
{
}