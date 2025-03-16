#include "util/enums/accounting/t_accounts.hpp"

using namespace util::enums;

std::map<TAccounts, std::string> util::enums::t_accounts_name_map = {
    // assets
    {TAccounts::CASH, "Cash"},
    {TAccounts::EQUIPMENT, "Equipment"},
    {TAccounts::ACCTRCV, "Accounts Receivable"},
    {TAccounts::NOTERCV, "Notes Receivable"},
    {TAccounts::LAND, "Land"},
    {TAccounts::ACCUMDEPRECIATION, "Accumulated Depreciation"},
    {TAccounts::INVENTORY, "Inventory"},
    // liabilities
    {TAccounts::ACCPAYABLE, "Accounts Payable"},
    {TAccounts::NOTEPAYABLE, "Notes Payable"},
    {TAccounts::UNEARNEDREV, "Unearned Revenue"},
    // stockholders equity
    {TAccounts::COMMONSTOCK, "Common Stock"},
    {TAccounts::ADDTLCPT, "Additional Paid-in Capital"},
    {TAccounts::OWNEREQ, "Owners Equity"},
    {TAccounts::DIVIDEND, "Dividends"},
    {TAccounts::REV, "Revenue"},
    {TAccounts::EXPENSE, "Expense"},
    {TAccounts::COGS, "Cost of Goods Sold"},
    {TAccounts::DEPREXP, "Depreciation Expense"},
    {TAccounts::WAGEEXP, "Wages Expense"},
    {TAccounts::RETAINEDEARNINGS, "Retained Earnings"},
    {TAccounts::END, "HOLD"}};

std::map<std::string, TAccounts> util::enums::t_accounts_name_map_inversed = {
    // assets
    {"Cash", TAccounts::CASH},
    {"Equipment", TAccounts::EQUIPMENT},
    {"Accounts Receivable", TAccounts::ACCTRCV},
    {"Notes Receivable", TAccounts::NOTERCV},
    {"Land", TAccounts::LAND},
    {"Accumulated Depreciation", TAccounts::ACCUMDEPRECIATION},
    {"Inventory", TAccounts::INVENTORY},
    // liabilities
    {"Accounts Payable", TAccounts::ACCPAYABLE},
    {"Notes Payable", TAccounts::NOTEPAYABLE},
    {"Unearned Revenue", TAccounts::UNEARNEDREV},
    // stockholders equity
    {"Common Stock", TAccounts::COMMONSTOCK},
    {"Additional Paid-in Capital", TAccounts::ADDTLCPT},
    {"Owners Equity", TAccounts::OWNEREQ},
    {"Dividends", TAccounts::DIVIDEND},
    {"Revenue", TAccounts::REV},
    {"Expense", TAccounts::EXPENSE},
    {"Cost of Goods Sold", TAccounts::COGS},
    {"Depreciation Expense", TAccounts::DEPREXP},
    {"Wages Expense", TAccounts::WAGEEXP},
    {"Retained Earnings", TAccounts::RETAINEDEARNINGS},
    {"HOLD", TAccounts::END}};

std::map<TAccounts, AccountTitles> util::enums::t_accounts_acc_title_map = {
    {TAccounts::CASH, AccountTitles::ASSETS},
    {TAccounts::EQUIPMENT, AccountTitles::ASSETS},
    {TAccounts::ACCTRCV, AccountTitles::ASSETS},
    {TAccounts::NOTERCV, AccountTitles::ASSETS},
    {TAccounts::LAND, AccountTitles::ASSETS},
    {TAccounts::ACCUMDEPRECIATION, AccountTitles::ASSETS},
    {TAccounts::INVENTORY, AccountTitles::ASSETS},
    {TAccounts::ACCPAYABLE, AccountTitles::LIABILITIES},
    {TAccounts::NOTEPAYABLE, AccountTitles::LIABILITIES},
    {TAccounts::UNEARNEDREV, AccountTitles::LIABILITIES},
    {TAccounts::COMMONSTOCK, AccountTitles::STOCKHOLDERSEQUITY},
    {TAccounts::ADDTLCPT, AccountTitles::STOCKHOLDERSEQUITY},
    {TAccounts::OWNEREQ, AccountTitles::STOCKHOLDERSEQUITY},
    {TAccounts::DIVIDEND, AccountTitles::STOCKHOLDERSEQUITY},
    {TAccounts::REV, AccountTitles::STOCKHOLDERSEQUITY},
    {TAccounts::EXPENSE, AccountTitles::STOCKHOLDERSEQUITY},
    {TAccounts::COGS, AccountTitles::STOCKHOLDERSEQUITY},
    {TAccounts::DEPREXP, AccountTitles::STOCKHOLDERSEQUITY},
    {TAccounts::WAGEEXP, AccountTitles::STOCKHOLDERSEQUITY},
    {TAccounts::RETAINEDEARNINGS, AccountTitles::STOCKHOLDERSEQUITY},
};

TAccounts &util::enums::operator++(TAccounts &t_account)
{
    if (t_account == LAST_TACCOUNT)
    {
        throw std::invalid_argument("Maximum value reached");
    }
    int int_equivalent = static_cast<int>(t_account) + 1;
    t_account = static_cast<TAccounts>(int_equivalent);
    return t_account;
}

std::vector<TAccounts> util::enums::assets_t_accounts = {
    TAccounts::CASH,
    TAccounts::EQUIPMENT,
    TAccounts::ACCTRCV,
    TAccounts::NOTERCV,
    TAccounts::LAND,
    TAccounts::ACCUMDEPRECIATION,
    TAccounts::INVENTORY};

std::vector<TAccounts> util::enums::liabilities_t_accounts = {
    TAccounts::ACCPAYABLE,
    TAccounts::NOTEPAYABLE,
    TAccounts::UNEARNEDREV};

std::vector<TAccounts> util::enums::stockholders_t_accounts = {
    TAccounts::COMMONSTOCK,
    TAccounts::ADDTLCPT,
    TAccounts::REV,
    TAccounts::EXPENSE,
    TAccounts::OWNEREQ,
    TAccounts::DIVIDEND,
    TAccounts::COGS,
    TAccounts::DEPREXP,
    TAccounts::WAGEEXP,
    TAccounts::RETAINEDEARNINGS};

std::string util::enums::get_name(TAccounts t_account)
{
    return t_accounts_name_map[t_account];
}

TAccounts util::enums::get_t_account_enum(std::string name)
{
    for (auto it = t_accounts_name_map.begin(); it != t_accounts_name_map.end(); it++)
    {
        if (it->second == name)
        {
            return it->first;
        }
    }
    throw std::invalid_argument(name + " does not match any T-account");
}

AccountTitles util::enums::get_account_title(TAccounts t_account_name)
{
    if (t_account_name < MAX_ASSET)
    {
        return AccountTitles::ASSETS;
    }
    if (t_account_name < MAX_LIABILITY)
    {
        return AccountTitles::LIABILITIES;
    }
    if (t_account_name < MAX_STOCKHOLDER)
    {
        return AccountTitles::STOCKHOLDERSEQUITY;
    }
    throw std::invalid_argument("T-Account doesn't exist");
}