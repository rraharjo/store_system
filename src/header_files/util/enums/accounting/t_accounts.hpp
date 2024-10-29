#include <iostream>
#include <string>
#include <map>
#include <vector>

#ifndef TACCOUNTS_HPP
#define TACCOUNTS_HPP
namespace util
{
    namespace enums
    {
        enum class TAccounts
        {
            //assets
            CASH = 0,
            EQUIPMENT,
            ACCTRCV,
            NOTERCV,
            LAND,
            ACCUMDEPRECIATION,
            INVENTORY,
            //liabilities
            ACCPAYABLE,
            NOTEPAYABLE,
            UNEARNEDREV,
            //stockholders equity
            COMMONSTOCK,
            ADDTLCPT,
            OWNEREQ,
            DIVIDEND,
            REV,
            EXPENSE,
            COGS,
            DEPREXP,
            WAGEEXP
        };

        extern std::map<TAccounts, std::string> tAccountsNameMap;

        extern std::vector<TAccounts> assetsTAccounts;
        
        extern std::vector<TAccounts> liabilitiesTAccounts;

        extern std::vector<TAccounts> stockholdersTAccounts;

        std::string getName(util::enums::TAccounts);

        TAccounts getTAccountEnum(std::string);
    };
};

#endif
