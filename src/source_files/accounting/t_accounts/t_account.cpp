#include "accounting/t_accounts/t_account.hpp"
using namespace accounting;

TAccount::TAccount(util::enums::TAccounts title)
{
    this->title = title;
    this->debitEntries = {};
    this->creditEntries = {};
}

util::enums::TAccounts TAccount::getTitle()
{
    return this->title;
}

std::string TAccount::getTitleName()
{
    return util::enums::tAccountsNameMap[this->title];
}

void TAccount::addEntry(Entry *entry)
{
    if (entry->isDebit())
    {
        this->debitEntries.push_back(entry);
    }
    else
    {
        this->creditEntries.push_back(entry);
    }
}

double TAccount::getDebitAmount()
{
    double total = 0;
    for (Entry *event : this->debitEntries)
    {
        total += event->getAmount();
    }
    return total;
}

double TAccount::getCreditAmount()
{
    double total = 0;
    for (Entry *event : this->creditEntries)
    {
        total += event->getAmount();
    }
    return total;
}

double TAccount::getTotalAmount()
{
    return this->getDebitAmount() - this->getCreditAmount();
}

std::string TAccount::to_string()
{
    std::string toRet = this->getTitleName() + "\n";
    for (Entry *entry : this->debitEntries)
    {
        toRet += entry->to_string();
    }
    for (Entry *entry : this->creditEntries)
    {
        toRet += entry->to_string();
    }
    return toRet;
}