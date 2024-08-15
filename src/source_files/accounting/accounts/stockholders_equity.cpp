#include "accounting/accounts/stockholders_equity.hpp"

using namespace accounting;

StockholdersEquityAccount::StockholdersEquityAccount() : Account(false, util::enums::accountTitlesMap[util::enums::AccountTitles::STOCKHOLDERSEQUITY])
{
    this->initiateTAccount();
}

void StockholdersEquityAccount::initiateTAccount()
{
    for (auto tAccount : util::enums::allStockholdersTAccounts)
    {
        this->addTAccount(new TAccount(util::enums::stockholdersTAccountNames[tAccount]));
    }
}