#include "accounting/accounts/assets.hpp"

using namespace accounting;

Assets::Assets() : Account(true, util::enums::accountTitlesMap[util::enums::AccountTitles::ASSETS])
{
    this->initiateTAccount();
}

void Assets::initiateTAccount()
{
    for (auto tAccount : util::enums::allAssetsTAccounts)
    {
        this->addTAccount(new TAccount(util::enums::assetTAccountNames[tAccount]));
    }
}