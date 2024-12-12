#include "store/transaction/transaction.hpp"
using namespace store;

Transaction::Transaction(util::Date *transactionDate, double paidCash, double paidCredit)
    : util::baseclass::HasTable()
{
    this->transactionDate = transactionDate;
    this->paidCash = paidCash;
    this->paidCredit = paidCredit;
    this->isFinished = this->paidCredit == 0.0 ? true : false;
    this->entries = {};
}

Transaction::Transaction(util::Date *transactionDate)
    : Transaction::Transaction(transactionDate, 0, 0)
{
}

double Transaction::getPaidCash()
{
    return this->paidCash;
}

double Transaction::getPaidCredit()
{
    return this->paidCredit;
}

double Transaction::getTransactionAmount()
{
    double toRet = 0.0;
    for (inventory::Entry *entry : this->entries)
    {
        toRet += entry->getPrice() * entry->getQty();
    }
    return toRet;
}

util::Date *Transaction::getDate()
{
    return this->transactionDate;
}

void Transaction::addEntry(inventory::Entry *entry)
{
    this->entries.push_back(entry);
}

void Transaction::setPaidCash(double amount)
{
    this->paidCash = amount;
    this->isFinished = this->paidCash == this->getTransactionAmount() ? true : false;
}

void Transaction::setPaidCredit(double amount)
{
    this->paidCredit = amount;
    this->isFinished = this->paidCredit == 0 ? true : false;
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
    for (inventory::Entry *entry : this->getAllEntries())
    {
        entry->setTransactionDBCode(this->getDBCode());
        entry->insertToDB();
    }
};

void PurchaseTransaction::updateToDB()
{
    this->updateToDBWithTable(PurchaseTransaction::classTable);
};

PurchaseTransaction::PurchaseTransaction(std::string seller, util::Date *purchaseDate) : Transaction::Transaction(purchaseDate)
{
    this->seller = seller;
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
    args.push_back(std::to_string(this->getPaidCash()));
    args.push_back(std::to_string(this->getPaidCredit()));
    args.push_back(this->isFinished ? "true" : "false");
    return args;
}

std::vector<std::string> PurchaseTransaction::getUpdateParameter()
{
    std::vector<std::string> args;
    args.push_back(this->getDate()->toDBFormat());
    args.push_back(this->getSeller());
    args.push_back(std::to_string(this->getPaidCash()));
    args.push_back(std::to_string(this->getPaidCredit()));
    args.push_back(this->isFinished ? "true" : "false");
    return args;
}

/*********************SELLINGTRANSACTION***********************/

util::Table *SellingTransaction::classTable = util::SellingTransactionTable::getInstance();

void SellingTransaction::insertToDB()
{
    this->insertToDBWithTable(SellingTransaction::classTable);
    for (inventory::Entry *entry : this->getAllEntries())
    {
        entry->setTransactionDBCode(this->getDBCode());
        entry->insertToDB();
    }
};

void SellingTransaction::updateToDB()
{
    this->updateToDBWithTable(SellingTransaction::classTable);
};

SellingTransaction::SellingTransaction(util::Date *transactionDate) : Transaction::Transaction(transactionDate)
{
}

std::vector<std::string> SellingTransaction::getInsertParameter()
{
    std::vector<std::string> args;
    args.push_back(util::enums::primaryKeyCodesMap[util::enums::PrimaryKeyCodes::SELLINGTRANSACTION]);
    args.push_back(this->getDate()->toDBFormat());
    args.push_back(std::to_string(this->getPaidCash()));
    args.push_back(std::to_string(this->getPaidCredit()));
    args.push_back(this->isFinished ? "true" : "false");
    return args;
}

std::vector<std::string> SellingTransaction::getUpdateParameter()
{
    std::vector<std::string> args;
    args.push_back(this->getDate()->toDBFormat());
    args.push_back(std::to_string(this->getPaidCash()));
    args.push_back(std::to_string(this->getPaidCredit()));
    args.push_back(this->isFinished ? "true" : "false");
    return args;
}