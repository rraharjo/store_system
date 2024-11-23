#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "util/enums/accounting/account_titles.hpp"

#ifndef TACCOUNTS_HPP
#define TACCOUNTS_HPP
#define FIRST_TACCOUNT util::enums::TAccounts::CASH
#define MAX_ASSET util::enums::TAccounts::ACCPAYABLE
#define MAX_LIABILITY util::enums::TAccounts::COMMONSTOCK
#define MAX_STOCKHOLDER util::enums::TAccounts::END
#define MIN_TEMPORARY_ACCOUNT util::enums::TAccounts::REV
#define MAX_TEMPORARY_ACCOUNT util::enums::TAccounts::RETAINEDEARNINGS
#define LAST_TACCOUNT util::enums::TAccounts::END
namespace util
{
    namespace enums
    {

        enum class TAccounts
        {
            // assets
            CASH = 0,
            EQUIPMENT,
            ACCTRCV,
            NOTERCV,
            LAND,
            ACCUMDEPRECIATION,
            INVENTORY,
            // liabilities
            ACCPAYABLE,
            NOTEPAYABLE,
            UNEARNEDREV,
            // stockholders equity
            COMMONSTOCK,
            ADDTLCPT,
            OWNEREQ,
            DIVIDEND,
            REV,
            EXPENSE,
            COGS,
            DEPREXP,
            WAGEEXP,
            RETAINEDEARNINGS,
            // END placeholder
            END
        };

        TAccounts &operator++(TAccounts &tAccount);

        extern std::map<TAccounts, std::string> tAccountsNameMap;

        extern std::vector<TAccounts> assetsTAccounts;

        extern std::vector<TAccounts> liabilitiesTAccounts;

        extern std::vector<TAccounts> stockholdersTAccounts;

        std::string getName(util::enums::TAccounts);

        TAccounts getTAccountEnum(std::string);

        AccountTitles getAccountTitle(util::enums::TAccounts);
    };
};

#endif
