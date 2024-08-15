#include "accounting/accounting_transaction/transaction.hpp"
using namespace accounting;

Transaction::Transaction(std::string name)
{
    this->name = name;
    this->debitEntries = {};
    this->creditEntries = {};
    this->timeStamp = util::Date::getUniqueTimestamp();
}

long long Transaction::getTimestamp()
{
    return this->timeStamp;
}

void Transaction::addEntry(Entry *entry)
{
    if ((*entry).isDebit())
    {
        this->debitEntries.push_back(entry);
    }
    else
    {
        this->creditEntries.push_back(entry);
    }
    (*entry).setTransactionRef(this->timeStamp);
    (*entry).setTransactionTitle(this->name);
}

double Transaction::getDebitAmount()
{
    // fold_left
    double total = 0;
    for (Entry *entry : this->debitEntries)
    {
        total += (*entry).getAmount();
    }
    return total;
}

double Transaction::getCreditAmount()
{
    // fold_left
    double total = 0;
    for (Entry *entry : this->creditEntries)
    {
        total += (*entry).getAmount();
    }
    return total;
}

bool Transaction::isBalanced()
{
    return this->getDebitAmount() == this->getCreditAmount();
}

std::vector<Entry *> &Transaction::getDebitEntries()
{
    return this->debitEntries;
}

std::vector<Entry *> &Transaction::getCreditEntries()
{
    return this->creditEntries;
}