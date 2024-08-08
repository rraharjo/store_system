#include <iostream>
#include "accounting/accounts/assets.hpp"

using namespace accounting;

Assets::Assets() : Account(true, enums::accountTitlesMap[enums::AccountTitles::ASSETS])
{
    this->initiateTAccount();
}

void Assets::initiateTAccount()
{
    for (auto tAccount : enums::allAssetsTAccounts)
    {
        this->addTAccount(new TAccount(enums::assetTAccountNames[tAccount]));
    }
}