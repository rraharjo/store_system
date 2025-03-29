#ifndef ACCOUNTTITLES_HPP
#define ACCOUNTTITLES_HPP
#include <iostream>
#include <string>
#include <map>


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

        extern std::map<AccountTitles, std::string> account_titles_map;
        
        extern AccountTitles all_account_titles[3];

        std::string get_name(util::enums::AccountTitles);

        AccountTitles get_account_enum(std::string);
    };
};

#endif
