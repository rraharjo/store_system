#include "accounting/enums/assets_t_accounts.hpp"

using namespace accounting;

std::map<enums::AssetTAccount, std::string> enums::assetTAccountNames = {
    {enums::CASH, "Cash"},
    {enums::EQUIPMENT, "Equipment"},
    {enums::ACCTRCV, "Account Receivable"},
    {enums::LAND, "Land"}};

enums::AssetTAccount enums::allAssetsTAccounts[] = {enums::CASH, enums::EQUIPMENT, enums::ACCTRCV, enums::LAND};

int enums::asInt(AssetTAccount tAccount)
{
    return static_cast<int>(tAccount);
}
