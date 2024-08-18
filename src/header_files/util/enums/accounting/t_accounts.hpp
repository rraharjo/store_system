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
        enum TAccounts
        {
            //assets
            CASH = 0,
            EQUIPMENT,
            ACCTRCV,
            LAND,
            ACCUMDEPRECIATION,
            //liabilities
            ACCPAYABLE,
            NOTEPAYABLE,
            UNEARNEDREV,
            //stockholders equity
            COMMONSTOCK,
            ADDTLCPT,
            REV,
            EXPENSE,
            DEPRECIATIONEXPENSE
        };

        extern std::map<TAccounts, std::string> tAccountsNameMap;

        extern std::vector<TAccounts> assetsTAccounts;
        
        extern std::vector<TAccounts> liabilitiesTAccounts;

        extern std::vector<TAccounts> stockholdersTAccounts;
    };
};

#endif
