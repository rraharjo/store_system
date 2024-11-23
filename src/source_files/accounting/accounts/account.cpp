#include "accounting/accounts/account.hpp"
using namespace accounting;

void Account::addTAccount(util::enums::TAccounts tAccount)
{
    this->tAccounts[tAccount] = TAccount::generateFromDatabase(tAccount);
}

Account::Account(bool debit, util::enums::AccountTitles title)
{
    this->debit = debit;
    this->title = title;
    this->tAccounts = {};
}

Account::~Account()
{
    for (auto it = this->tAccounts.begin(); it != this->tAccounts.end(); it++)
    {
        delete it->second;
    }
}

util::enums::AccountTitles Account::getTitle()
{
    return this->title;
}

std::string Account::getTitleName()
{
    return util::enums::getName(this->title);
}

double Account::getTotalCredit()
{
    double toRet = 0.0;
    for (auto it = this->tAccounts.begin(); it != this->tAccounts.end(); it++)
    {
        toRet += it->second->getCreditAmount();
    }
    return toRet;
}

double Account::getTotalDebit()
{
    double toRet = 0.0;
    for (auto it = this->tAccounts.begin(); it != this->tAccounts.end(); it++)
    {
        toRet += it->second->getDebitAmount();
    }
    return toRet;
}

void Account::addEntry(Entry *entry)
{
    this->tAccounts[entry->getTAccount()]->addEntry(entry);
    // entry->insertToDB();
}

std::string Account::to_string()
{
    std::string toRet = this->getTitleName();
    toRet += "\n";
    int num = 1;
    for (auto it = this->tAccounts.begin(); it != this->tAccounts.end(); it++)
    {
        toRet += std::to_string(num++) + ". " + it->second->to_string();
    }
    return toRet;
}

Assets::Assets() : Account(true, util::enums::AccountTitles::ASSETS)
{
    this->initiateTAccount();
}

void Assets::initiateTAccount()
{
    for (util::enums::TAccounts tAccount : util::enums::assetsTAccounts)
    {
        this->addTAccount(tAccount);
    }
}

Liabilities::Liabilities() : Account(false, util::enums::AccountTitles::LIABILITIES)
{
    this->initiateTAccount();
}

void Liabilities::initiateTAccount()
{
    for (util::enums::TAccounts tAccount : util::enums::liabilitiesTAccounts)
    {
        this->addTAccount(tAccount);
    }
}

StockholdersEquityAccount::StockholdersEquityAccount() : Account(false, util::enums::AccountTitles::STOCKHOLDERSEQUITY)
{
    this->initiateTAccount();
}

void StockholdersEquityAccount::initiateTAccount()
{
    for (util::enums::TAccounts tAccount : util::enums::stockholdersTAccounts)
    {
        this->addTAccount(tAccount);
    }
}

std::vector<TAccount *> StockholdersEquityAccount::getTemporaryAccounts()
{
    std::vector<TAccount *> tAccounts;
    for (util::enums::TAccounts currentAccount = MIN_TEMPORARY_ACCOUNT;
         currentAccount < MAX_TEMPORARY_ACCOUNT; ++currentAccount)
    {
        tAccounts.push_back(this->tAccounts[currentAccount]);
    }
    return tAccounts;
}