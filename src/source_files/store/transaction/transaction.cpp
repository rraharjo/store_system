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
    //this->setDBCode(this->createDBCode());
    this->insertToDB();
}

std::string PurchaseTransaction::getSeller()
{
    return this->seller;
}

std::vector<std::string> PurchaseTransaction::getInsertParameter()
{
    std::vector<std::string> args;
    args.push_back(util::enums::primaryKeyCodesMap[util::enums::PrimaryKeyCodes::PURCHASETRANSACTION]);
    args.push_back(this->getDate()->toDBFormat());
    args.push_back(this->getSeller());
    args.push_back(this->isFinished ? "true" : "false");
    return args;
}

std::vector<std::string> PurchaseTransaction::getUpdateParameter()
{
    std::vector<std::string> args;
    args.push_back(this->getDate()->toDBFormat());
    args.push_back(this->getSeller());
    args.push_back(this->isFinished ? "true" : "false");
    return args;
}

/*********************SELLINGTRANSACTION***********************/

util::Table *SellingTransaction::classTable = util::SellingTransactionTable::getInstance();

void SellingTransaction::insertToDB()
{
    this->insertToDBWithTable(SellingTransaction::classTable);
};

void SellingTransaction::updateToDB()
{
    this->updateToDBWithTable(SellingTransaction::classTable);
};

SellingTransaction::SellingTransaction(util::Date *transactionDate) : Transaction::Transaction(transactionDate)
{
    //this->setDBCode(this->createDBCode());
    this->insertToDB();
}

std::vector<std::string> SellingTransaction::getInsertParameter()
{
    std::vector<std::string> args;
    args.push_back(util::enums::primaryKeyCodesMap[util::enums::PrimaryKeyCodes::SELLINGTRANSACTION]);
    args.push_back(this->getDate()->toDBFormat());
    args.push_back(this->isFinished ? "true" : "false");
    return args;
}

std::vector<std::string> SellingTransaction::getUpdateParameter()
{
    std::vector<std::string> args;
    args.push_back(this->getDate()->toDBFormat());
    args.push_back(this->isFinished ? "true" : "false");
    return args;
}