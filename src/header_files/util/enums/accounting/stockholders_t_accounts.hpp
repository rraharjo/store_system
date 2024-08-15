#include <iostream>
#include <string>
#include <map>

#ifndef STOCKHOLDERSTACCOUNTS_HPP
#define STOCKHOLDERSTACCOUNTS_HPP
namespace util
{
    namespace enums
    {
        enum StockholdersTAccount
        {
            COMMONSTOCK = 0,
            ADDTLCPT = 1,
            REV = 2,
            EXPENSE = 3,
            DEPRECIATIONEXPENSE = 4
        };

        extern std::map<StockholdersTAccount, std::string> stockholdersTAccountNames;
        
        extern StockholdersTAccount allStockholdersTAccounts[5];

        int asInt(StockholdersTAccount tAccount);
    };
};
#endif