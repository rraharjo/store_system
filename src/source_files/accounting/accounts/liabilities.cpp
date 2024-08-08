#include <iostream>
#include "accounting/accounts/liabilities.hpp"

using namespace accounting;

Liabilities::Liabilities() : Account(false, enums::accountTitlesMap[enums::AccountTitles::LIABILITIES])
{
    this->initiateTAccount();
}

void Liabilities::initiateTAccount()
{
    for (auto tAccount : enums::allLiabilitiesTAccounts)
    {
        this->addTAccount(new TAccount(enums::liabilitiesTAccountNames[tAccount]));
    }
}