#include "accounting/accounts/account.hpp"
using namespace accounting;

void Account::addTAccount(TAccount *tAccount)
{
    this->tAccounts.push_back(tAccount);
}

Account::Account(bool debit, std::string name)
{
    this->debit = debit;
    this->tAccounts = {};
    this->name = name;
}

std::string Account::getName()
{
    return this->name;
}

void Account::addEntry(Entry *entry)
{
    (*this->tAccounts[(*entry).getTAccount()]).addEntry(entry);
}

std::string Account::to_string()
{
    std::string toRet = this->getName();
    toRet += "\n";
    int num = 1;
    for (TAccount *tAccount : this->tAccounts)
    {
        toRet += std::to_string(num++) + ". " + (*tAccount).to_string();
    }
    return toRet;
}
