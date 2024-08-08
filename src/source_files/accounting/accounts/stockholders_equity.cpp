#include <iostream>
#include "accounting/accounts/stockholders_equity.hpp"

using namespace accounting;

StockholdersEquityAccount::StockholdersEquityAccount() : Account(false, enums::accountTitlesMap[enums::AccountTitles::STOCKHOLDERSEQUITY])
{
    this->initiateTAccount();
}

void StockholdersEquityAccount::initiateTAccount()
{
    for (auto tAccount : enums::allStockholdersTAccounts)
    {
        this->addTAccount(new TAccount(enums::stockholdersTAccountNames[tAccount]));
    }
}