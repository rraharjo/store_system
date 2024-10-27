#include "accounting/accounting_system.hpp"

using namespace accounting;
AccountingSystem::AccountingSystem()
{
    this->assets = new Assets();
    this->liabilities = new Liabilities();
    this->stockholdersEquity = new StockholdersEquityAccount();
}

AccountingSystem::~AccountingSystem()
{
    delete this->assets;
    delete this->liabilities;
    delete this->stockholdersEquity;
}

void AccountingSystem::addEntry(Entry *entry)
{
    switch (entry->getAccountTitle())
    {
    case util::enums::AccountTitles::ASSETS:
        this->assets->addEntry(entry);
        break;
    case util::enums::AccountTitles::LIABILITIES:
        this->liabilities->addEntry(entry);
        break;
    case util::enums::AccountTitles::STOCKHOLDERSEQUITY:
        this->stockholdersEquity->addEntry(entry);
        break;
    }
}

AccountingSystem *AccountingSystem::getInstance()
{
    if (AccountingSystem::instance == NULL)
    {
        AccountingSystem::instance = new AccountingSystem();
    }
    return AccountingSystem::instance;
}

void AccountingSystem::addTransaction(Transaction *transaction)
{
    for (Entry *entry : transaction->getDebitEntries())
    {
        this->addEntry(entry);
    }
    for (Entry *entry : transaction->getCreditEntries())
    {
        this->addEntry(entry);
    }
    this->transactions[transaction->getDBCode()] = transaction;
}

std::string AccountingSystem::to_string()
{
    std::string toRet = "";
    toRet += this->assets->to_string();
    toRet += "\n";
    toRet += this->liabilities->to_string();
    toRet += "\n";
    toRet += this->stockholdersEquity->to_string();
    return toRet;
}

AccountingSystem *AccountingSystem::instance = NULL;