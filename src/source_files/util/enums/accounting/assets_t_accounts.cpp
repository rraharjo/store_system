#include "util/enums/accounting/assets_t_accounts.hpp"

using namespace util::enums;

std::map<AssetTAccount, std::string> util::enums::assetTAccountNames = {
    {CASH, "Cash"},
    {EQUIPMENT, "Equipment"},
    {ACCTRCV, "Account Receivable"},
    {LAND, "Land"}};

AssetTAccount util::enums::allAssetsTAccounts[] = {CASH, EQUIPMENT, ACCTRCV, LAND};

int util::enums::asInt(AssetTAccount tAccount)
{
    return static_cast<int>(tAccount);
}
