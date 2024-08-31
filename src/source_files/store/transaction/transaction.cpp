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

std::vector<inventory::Entry*> Transaction::getAllEntries(){
    return this->entries;
}

PurchaseTransaction::PurchaseTransaction(std::string seller, util::Date *purchaseDate) : Transaction::Transaction(purchaseDate)
{
    this->setTable();
    this->seller = seller;
    this->insertToDB();
}

std::string PurchaseTransaction::getSeller(){
    return this->seller;
}

void PurchaseTransaction::setTable()
{
    this->table = util::PurchaseTransactionTable::getInstance();
}

std::vector<std::string> PurchaseTransaction::getInsertParameter()
{
    std::vector<std::string> args;
    args.push_back(this->getDate()->toDBFormat());
    args.push_back(this->getSeller());
    return args;
}

SellingTransaction::SellingTransaction(util::Date *transactionDate) : Transaction::Transaction(transactionDate)
{
    this->setTable();
    insertToDB();
}

void SellingTransaction::setTable()
{
    this->table = util::SellingTransactionTable::getInstance();
}

std::vector<std::string> SellingTransaction::getInsertParameter()
{
    std::vector<std::string> args;
    args.push_back(this->getDate()->to_string());
    return args;
}