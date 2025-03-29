#ifndef TACCOUNTS_HPP
#define TACCOUNTS_HPP
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "util/enums/accounting/account_titles.hpp"


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

        TAccounts &operator++(TAccounts &t_account);

        extern std::map<TAccounts, std::string> t_accounts_name_map;

        extern std::map<std::string, TAccounts> t_accounts_name_map_inversed;

        extern std::map<TAccounts, AccountTitles> t_accounts_acc_title_map;

        extern std::vector<TAccounts> assets_t_accounts;

        extern std::vector<TAccounts> liabilities_t_accounts;

        extern std::vector<TAccounts> stockholders_t_accounts;

        std::string get_name(util::enums::TAccounts);

        TAccounts get_t_account_enum(std::string);

        AccountTitles get_account_title(util::enums::TAccounts);
    };
};

#endif
