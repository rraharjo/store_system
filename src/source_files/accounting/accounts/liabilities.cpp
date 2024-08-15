#include <iostream>
#include "accounting/accounts/liabilities.hpp"

using namespace accounting;

Liabilities::Liabilities() : Account(false, util::enums::accountTitlesMap[util::enums::AccountTitles::LIABILITIES])
{
    this->initiateTAccount();
}

void Liabilities::initiateTAccount()
{
    for (auto tAccount : util::enums::allLiabilitiesTAccounts)
    {
        this->addTAccount(new TAccount(util::enums::liabilitiesTAccountNames[tAccount]));
    }
}