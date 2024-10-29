#include <iostream>
#include <string>
#include <map>

#ifndef ACCOUNTTITLES_HPP
#define ACCOUNTTITLES_HPP
namespace util
{
    namespace enums
    {
        enum class AccountTitles
        {
            ASSETS = 0,
            LIABILITIES = 1,
            STOCKHOLDERSEQUITY = 2
        };

        extern std::map<AccountTitles, std::string> accountTitlesMap;
        
        extern AccountTitles allAccountTitles[3];

        std::string getName(util::enums::AccountTitles);

        AccountTitles getAccountEnum(std::string);
    };
};

#endif
