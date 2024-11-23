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

void AccountingSystem::addExistingTransaction(Transaction *transaction)
{
    this->addTransaction(transaction);
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
        TAccount::initiateTAccountOnDB();
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

void AccountingSystem::endYearAdjustment()//TO DO: adjust the closing the book entries for all temporary t-accounts
{
    util::Date *now = new util::Date();
    std::string transactionTitle = "Closing the book";
    std::vector<TAccount *> temporaryAccounts = this->stockholdersEquity->getTemporaryAccounts();
    accounting::Transaction *closeTheBook =
        util::factory::ClosingTemporaryAccountsFactory(now, transactionTitle, temporaryAccounts)
            .createTransaction();
    this->addTransaction(closeTheBook);
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