#include <iostream>
#include <string>
#include <map>

#ifndef ACCOUNTTITLES_HPP
#define ACCOUNTTITLES_HPP
namespace accounting
{
    namespace enums
    {
        enum AccountTitles
        {
            ASSETS = 0,
            LIABILITIES = 1,
            STOCKHOLDERSEQUITY = 2
        };

        extern std::map<AccountTitles, std::string> accountTitlesMap;
        
        extern AccountTitles allAccountTitles[3];

        std::string getName(int title);
    };
};

#endif
