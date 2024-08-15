#include <iostream>
#include <string>
#include <map>

#ifndef LIABILITIESTACCOUNTS_HPP
#define LIABILITIESTACCOUNTS_HPP
namespace util
{
    namespace enums
    {
        enum LiabilitiesTAccount
        {
            ACCPAYABLE = 0,
            NOTEPAYABLE = 1,
            UNEARNEDREV = 2
        };

        extern std::map<LiabilitiesTAccount, std::string> liabilitiesTAccountNames;

        extern LiabilitiesTAccount allLiabilitiesTAccounts[3];

        int asInt(LiabilitiesTAccount tAccount);
    };
};
#endif
