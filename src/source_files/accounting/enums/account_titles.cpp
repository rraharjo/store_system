#include "accounting/enums/account_titles.hpp"

using namespace accounting;

std::map<enums::AccountTitles, std::string> enums::accountTitlesMap = {
    {enums::ASSETS, "Assets"},
    {enums::LIABILITIES, "Liabilities"},
    {enums::STOCKHOLDERSEQUITY, "Stockholders Equity"}};

enums::AccountTitles enums::allAccountTitles[] = {enums::ASSETS, enums::LIABILITIES, enums::STOCKHOLDERSEQUITY};

std::string enums::getName(int title)
{
    AccountTitles temp = static_cast<AccountTitles>(title);
    return enums::accountTitlesMap[temp];
}