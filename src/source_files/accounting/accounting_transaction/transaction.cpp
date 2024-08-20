#include "accounting/accounting_transaction/transaction.hpp"
using namespace accounting;

Transaction::Transaction(std::string name, util::Date* transactionDate)
{
    this->name = name;
    this->debitEntries = {};
    this->creditEntries = {};
    this->transactionDate = transactionDate;
}

Transaction::Transaction(std::string name){
    this->name = name;
    this->debitEntries = {};
    this->creditEntries = {};
    this->transactionDate = new util::Date();
}

void Transaction::setDBCode(int dbCode){
    this->dbCode = dbCode;
}

void Transaction::setTable() {
    this->table = util::AccountingTransactionTable::getInstance();
}

int Transaction::getDBCode(){
    return this->dbCode;
}

std::vector<Entry *> &Transaction::getDebitEntries()
{
    return this->debitEntries;
}

std::vector<Entry *> &Transaction::getCreditEntries()
{
    return this->creditEntries;
}

double Transaction::getDebitAmount()
{
    double total = 0;
    for (Entry *entry : this->debitEntries)
    {
        total += (*entry).getAmount();
    }
    return total;
}

double Transaction::getCreditAmount()
{
    double total = 0;
    for (Entry *entry : this->creditEntries)
    {
        total += (*entry).getAmount();
    }
    return total;
}

void Transaction::addEntry(Entry *entry)
{
    if (entry->isDebit())
    {
        this->debitEntries.push_back(entry);
    }
    else
    {
        this->creditEntries.push_back(entry);
    }
    entry->setTransactionDB(this->dbCode);
    entry->setTransactionTitle(this->name);
}



bool Transaction::isBalanced()
{
    return this->getDebitAmount() == this->getCreditAmount();
}

