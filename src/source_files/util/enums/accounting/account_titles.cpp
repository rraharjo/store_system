#include "util/enums/accounting/account_titles.hpp"

using namespace util::enums;

std::map<util::enums::AccountTitles, std::string> util::enums::accountTitlesMap = {
    {util::enums::AccountTitles::ASSETS, "Assets"},
    {util::enums::AccountTitles::LIABILITIES, "Liabilities"},
    {util::enums::AccountTitles::STOCKHOLDERSEQUITY, "Stockholders Equity"}};

AccountTitles util::enums::allAccountTitles[] = {
    util::enums::AccountTitles::ASSETS,
    util::enums::AccountTitles::LIABILITIES,
    util::enums::AccountTitles::STOCKHOLDERSEQUITY};

std::string util::enums::getName(util::enums::AccountTitles title)
{
    return accountTitlesMap[title];
}

AccountTitles util::enums::getAccountEnum(std::string name){
    for (auto it = accountTitlesMap.begin() ; it != accountTitlesMap.end() ; it++){
        if (it->second == name){
            return it->first;
        }
    }
    throw std::invalid_argument(name + " does not match any account");
}