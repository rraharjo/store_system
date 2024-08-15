#include "util/enums/accounting/account_titles.hpp"

using namespace util::enums;

std::map<util::enums::AccountTitles, std::string> util::enums::accountTitlesMap = {
    {ASSETS, "Assets"},
    {LIABILITIES, "Liabilities"},
    {STOCKHOLDERSEQUITY, "Stockholders Equity"}};

AccountTitles util::enums::allAccountTitles[] = {ASSETS, LIABILITIES, STOCKHOLDERSEQUITY};

std::string util::enums::getName(int title)
{
    AccountTitles temp = static_cast<AccountTitles>(title);
    return accountTitlesMap[temp];
}