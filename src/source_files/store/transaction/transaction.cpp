#include "store/transaction/transaction.hpp"
using namespace store;

Transaction::Transaction(util::Date *transactionDate)
{
    this->transactionDate = transactionDate;
    this->entries = {};
}

util::Date *Transaction::getDate()
{
    return this->transactionDate;
}

void Transaction::addEntry(inventory::Entry *entry)
{
    this->entries.push_back(entry);
}

std::vector<inventory::Entry *> Transaction::getAllEntries()
{
    return this->entries;
}

/****************************************************/
int PurchaseTransaction::nextItemCode = 0;

PurchaseTransaction::PurchaseTransaction(std::string seller, util::Date *purchaseDate) : Transaction::Transaction(purchaseDate)
{
    this->setTable();
    this->seller = seller;
    this->setDBCode(this->createDBCode());
    this->insertToDB();
}

std::string PurchaseTransaction::getSeller()
{
    return this->seller;
}

void PurchaseTransaction::setTable()
{
    this->table = util::PurchaseTransactionTable::getInstance();
}

std::vector<std::string> PurchaseTransaction::getInsertParameter()
{
    std::vector<std::string> args;
    args.push_back(this->getDBCode());
    args.push_back(this->getDate()->toDBFormat());
    args.push_back(this->getSeller());
    return args;
}

std::string PurchaseTransaction::createDBCode()
{
    char numAsString[6];
    sprintf(numAsString, "%05d", PurchaseTransaction::nextItemCode++);
    std::string countAsString = numAsString;
    std::string dbCode = "PTR" + countAsString;
    return dbCode;
}

/********************************************/
int SellingTransaction::nextItemCode = 0;

std::string SellingTransaction::createDBCode()
{
    char numAsString[6];
    sprintf(numAsString, "%05d", SellingTransaction::nextItemCode++);
    std::string countAsString = numAsString;
    std::string dbCode = "STR" + countAsString;
    return dbCode;
}

SellingTransaction::SellingTransaction(util::Date *transactionDate) : Transaction::Transaction(transactionDate)
{
    this->setTable();
    this->setDBCode(this->createDBCode());
    this->insertToDB();
}

void SellingTransaction::setTable()
{
    this->table = util::SellingTransactionTable::getInstance();
}

std::vector<std::string> SellingTransaction::getInsertParameter()
{
    std::vector<std::string> args;
    args.push_back(this->getDBCode());
    args.push_back(this->getDate()->toDBFormat());
    return args;
}