#include <iostream>
#include <string>
#include <map>

#ifndef ASSETSTACCOUNTS_HPP
#define ASSETSTACCOUNTS_HPP
namespace util
{
    namespace enums
    {
        enum AssetTAccount
        {
            CASH = 0,
            EQUIPMENT = 1,
            ACCTRCV = 2,
            LAND = 3,
            ACCUMDEPRECIATION = 4
        };

        extern std::map<AssetTAccount, std::string> assetTAccountNames;

        extern AssetTAccount allAssetsTAccounts[5];

        int asInt(AssetTAccount tAccount);
    };
};
#endif
