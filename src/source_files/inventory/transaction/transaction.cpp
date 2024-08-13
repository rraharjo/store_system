#include "inventory/transaction/transaction.hpp"
using namespace inventory;

Transaction::Transaction(util::Date *transactionDate)
{
    this->transactionDate = transactionDate;
    this->entries = {};
}

util::Date *Transaction::getDate()
{
    return this->transactionDate;
}

void Transaction::addEntry(int itemDBCode, int qty, double price)
{
    TransactionEntry *newEntry = new TransactionEntry();
    newEntry->itemDBCode = itemDBCode;
    newEntry->qty = qty;
    newEntry->price;
    newEntry->transactionDBCode = this->dbCode;
    this->entries.push_back(newEntry);
    // insert to database when added to sellable
}

PurchaseTransaction::PurchaseTransaction(std::string seller, util::Date *purchaseDate) : Transaction::Transaction(purchaseDate)
{
    this->setTable();
    this->seller = seller;
    std::vector<std::string> args;
    args.push_back(this->getDate()->to_string());
    args.push_back(this->seller);
    this->dbCode = this->insertToDB(args);
}

void PurchaseTransaction::setTable(){
    this->table = util::PurchaseTransactionTable::getInstance();
}

SellingTransaction::SellingTransaction(util::Date *transactionDate) : Transaction::Transaction(transactionDate)
{
    this->setTable();
    std::vector<std::string> args;
    args.push_back(this->getDate()->to_string());
    this->dbCode = this->insertToDB(args);
}

void SellingTransaction::setTable(){
    this->table = util::SellingTransactionTable::getInstance();
}