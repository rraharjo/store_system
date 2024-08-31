#include "util/enums/accounting/t_accounts.hpp"

using namespace util::enums;

std::map<TAccounts, std::string> util::enums::tAccountsNameMap = {
    //assets
    {util::enums::TAccounts::CASH, "Cash"},
    {util::enums::TAccounts::EQUIPMENT, "Equipment"},
    {util::enums::TAccounts::ACCTRCV, "Accounts Receivable"},
    {util::enums::TAccounts::NOTERCV, "Notes Receivable"},
    {util::enums::TAccounts::LAND, "Land"},
    {util::enums::TAccounts::ACCUMDEPRECIATION, "Accumulated Depreciation"},
    {util::enums::TAccounts::INVENTORY, "Inventory"},
    //liabilities
    {util::enums::TAccounts::ACCPAYABLE, "Accounts Payable"},
    {util::enums::TAccounts::NOTEPAYABLE, "Notes Payable"},
    {util::enums::TAccounts::UNEARNEDREV, "Unearned Revenue"},
    //stockholders equity
    {util::enums::TAccounts::COMMONSTOCK, "Common Stock"},
    {util::enums::TAccounts::ADDTLCPT, "Additional Paid-in Capital"},
    {util::enums::TAccounts::REV, "Revenue"},
    {util::enums::TAccounts::EXPENSE, "Expense"},
    {util::enums::TAccounts::OWNEREQ, "Owner's Equity"},
    {util::enums::TAccounts::DIVIDEND, "Dividends"},
    {util::enums::TAccounts::COGS, "Cost of Goods Sold"},
    {util::enums::TAccounts::DEPREXP, "Depreciation Expense"},
    {util::enums::TAccounts::WAGEEXP, "Wages Expense"},
};

std::vector<TAccounts> util::enums::assetsTAccounts = {
    util::enums::TAccounts::CASH,
    util::enums::TAccounts::EQUIPMENT,
    util::enums::TAccounts::ACCTRCV,
    util::enums::TAccounts::NOTERCV,
    util::enums::TAccounts::LAND,
    util::enums::TAccounts::ACCUMDEPRECIATION,
    util::enums::TAccounts::INVENTORY
};

std::vector<TAccounts> util::enums::liabilitiesTAccounts = {
    util::enums::TAccounts::ACCPAYABLE,
    util::enums::TAccounts::NOTEPAYABLE,
    util::enums::TAccounts::UNEARNEDREV
};

std::vector<TAccounts> util::enums::stockholdersTAccounts = {
    util::enums::TAccounts::COMMONSTOCK,
    util::enums::TAccounts::ADDTLCPT,
    util::enums::TAccounts::REV,
    util::enums::TAccounts::EXPENSE,
    util::enums::TAccounts::OWNEREQ,
    util::enums::TAccounts::DIVIDEND,
    util::enums::TAccounts::COGS,
    util::enums::TAccounts::DEPREXP,
    util::enums::TAccounts::WAGEEXP
};

std::string util::enums::getName(util::enums::TAccounts tAccount){
    return tAccountsNameMap[tAccount];
}