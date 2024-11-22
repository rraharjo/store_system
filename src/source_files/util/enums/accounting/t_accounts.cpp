#include "util/enums/accounting/t_accounts.hpp"

using namespace util::enums;

std::map<TAccounts, std::string> util::enums::tAccountsNameMap = {
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
    {TAccounts::OWNEREQ, "Owner's Equity"},
    {TAccounts::DIVIDEND, "Dividends"},
    {TAccounts::REV, "Revenue"},
    {TAccounts::EXPENSE, "Expense"},
    {TAccounts::COGS, "Cost of Goods Sold"},
    {TAccounts::DEPREXP, "Depreciation Expense"},
    {TAccounts::WAGEEXP, "Wages Expense"},
    {TAccounts::RETAINEDEARNINGS, "Retained Earnings"},
    {TAccounts::END, "HOLD"}};

TAccounts &util::enums::operator++(TAccounts &tAccount)
{
    if (tAccount == LAST_TACCOUNT){
        throw std::invalid_argument("Maximum value reached");
    }
    int intEqui = static_cast<int>(tAccount) + 1;
    tAccount = static_cast<TAccounts>(intEqui);
    return tAccount;
}

std::vector<TAccounts> util::enums::assetsTAccounts = {
    TAccounts::CASH,
    TAccounts::EQUIPMENT,
    TAccounts::ACCTRCV,
    TAccounts::NOTERCV,
    TAccounts::LAND,
    TAccounts::ACCUMDEPRECIATION,
    TAccounts::INVENTORY};

std::vector<TAccounts> util::enums::liabilitiesTAccounts = {
    TAccounts::ACCPAYABLE,
    TAccounts::NOTEPAYABLE,
    TAccounts::UNEARNEDREV};

std::vector<TAccounts> util::enums::stockholdersTAccounts = {
    TAccounts::COMMONSTOCK,
    TAccounts::ADDTLCPT,
    TAccounts::REV,
    TAccounts::EXPENSE,
    TAccounts::OWNEREQ,
    TAccounts::DIVIDEND,
    TAccounts::COGS,
    TAccounts::DEPREXP,
    TAccounts::WAGEEXP};

std::string util::enums::getName(TAccounts tAccount)
{
    return tAccountsNameMap[tAccount];
}

TAccounts util::enums::getTAccountEnum(std::string name)
{
    for (auto it = tAccountsNameMap.begin(); it != tAccountsNameMap.end(); it++)
    {
        if (it->second == name)
        {
            return it->first;
        }
    }
    throw std::invalid_argument(name + " does not match any T-account");
}

AccountTitles util::enums::getAccountTitle(TAccounts tAccountName)
{
    if (tAccountName < MAX_ASSET)
    {
        return AccountTitles::ASSETS;
    }
    if (tAccountName < MAX_LIABILITY)
    {
        return AccountTitles::LIABILITIES;
    }
    if (tAccountName < MAX_STOCKHOLDER){
        return AccountTitles::STOCKHOLDERSEQUITY;
    }
    throw std::invalid_argument("T-Account doesn't exist");
}