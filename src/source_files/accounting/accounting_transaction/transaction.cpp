#include "accounting/accounting_transaction/transaction.hpp"
using namespace accounting;

Transaction::Transaction(std::string name, util::Date *transactionDate) : util::baseclass::HasTable()
{
    this->setTable();
    this->name = name;
    this->debitEntries = {};
    this->creditEntries = {};
    this->transactionDate = transactionDate;
    this->insertToDB();
}

Transaction::Transaction(std::string name) : Transaction::Transaction(name, new util::Date())
{
}

Transaction::~Transaction()
{
    for (Entry *e : this->creditEntries)
    {
        if (!e->getDBCode())
        {
            delete e;
        }
    }

    for (Entry *e : this->debitEntries)
    {
        if (!e->getDBCode())
        {
            delete e;
        }
    }
}

void Transaction::setTable()
{
    this->table = util::AccountingTransactionTable::getInstance();
}

std::vector<std::string> Transaction::getInsertParameter()
{
    std::vector<std::string> args;
    args.push_back(this->name);
    args.push_back(this->transactionDate->toDBFormat());
    return args;
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
    entry->setTransactionDB(this->getDBCode());
    entry->setTransactionTitle(this->name);
}

bool Transaction::isBalanced()
{
    return this->getDebitAmount() == this->getCreditAmount();
}
