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

/************************PURCHASETRANSACTION****************************/
util::Table *PurchaseTransaction::classTable = util::PurchaseTransactionTable::getInstance();
int PurchaseTransaction::nextItemCode = 0;

void PurchaseTransaction::insertToDB()
{
    this->insertToDBWithTable(PurchaseTransaction::classTable);
};

void PurchaseTransaction::updateToDB()
{
    this->updateToDBWithTable(PurchaseTransaction::classTable);
};

PurchaseTransaction::PurchaseTransaction(std::string seller, util::Date *purchaseDate) : Transaction::Transaction(purchaseDate)
{
    this->seller = seller;
    this->setDBCode(this->createDBCode());
    this->insertToDB();
}

std::string PurchaseTransaction::getSeller()
{
    return this->seller;
}

std::vector<std::string> PurchaseTransaction::getInsertParameter()
{
    std::vector<std::string> args;
    args.push_back(this->getDBCode());
    args.push_back(this->getDate()->toDBFormat());
    args.push_back(this->getSeller());
    args.push_back(this->isFinished ? "true" : "false");
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

/*********************SELLINGTRANSACTION***********************/

util::Table *SellingTransaction::classTable = util::SellingTransactionTable::getInstance();
int SellingTransaction::nextItemCode = 0;

void SellingTransaction::insertToDB()
{
    this->insertToDBWithTable(SellingTransaction::classTable);
};

void SellingTransaction::updateToDB()
{
    this->updateToDBWithTable(SellingTransaction::classTable);
};

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
    this->setDBCode(this->createDBCode());
    this->insertToDB();
}

std::vector<std::string> SellingTransaction::getInsertParameter()
{
    std::vector<std::string> args;
    args.push_back(this->getDBCode());
    args.push_back(this->getDate()->toDBFormat());
    args.push_back(this->isFinished ? "true" : "false");
    return args;
}