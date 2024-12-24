#include "util/enums/accounting/account_titles.hpp"

using namespace util::enums;

std::map<util::enums::AccountTitles, std::string> util::enums::account_titles_map = {
    {util::enums::AccountTitles::ASSETS, "Assets"},
    {util::enums::AccountTitles::LIABILITIES, "Liabilities"},
    {util::enums::AccountTitles::STOCKHOLDERSEQUITY, "Stockholders Equity"}};

AccountTitles util::enums::all_account_titles[] = {
    util::enums::AccountTitles::ASSETS,
    util::enums::AccountTitles::LIABILITIES,
    util::enums::AccountTitles::STOCKHOLDERSEQUITY};

std::string util::enums::get_name(util::enums::AccountTitles title)
{
    return account_titles_map[title];
}

AccountTitles util::enums::get_account_enum(std::string name){
    for (auto it = account_titles_map.begin() ; it != account_titles_map.end() ; it++){
        if (it->second == name){
            return it->first;
        }
    }
    throw std::invalid_argument(name + " does not match any account");
}