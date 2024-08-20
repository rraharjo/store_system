#include "util/enums/accounting/t_accounts.hpp"

using namespace util::enums;

std::map<TAccounts, std::string> util::enums::tAccountsNameMap = {
    //assets
    {util::enums::TAccounts::CASH, "Cash"},
    {util::enums::TAccounts::EQUIPMENT, "Equipment"},
    {util::enums::TAccounts::ACCTRCV, "Account Receivable"},
    {util::enums::TAccounts::LAND, "Land"},
    //liabilities
    {util::enums::TAccounts::ACCPAYABLE, "Account Payable"},
    {util::enums::TAccounts::NOTEPAYABLE, "Notes Payable"},
    {util::enums::TAccounts::UNEARNEDREV, "Unearned Revenue"},
    //stockholders equity
    {util::enums::TAccounts::COMMONSTOCK, "Common Stock"},
    {util::enums::TAccounts::ADDTLCPT, "Additional Paid-in Capital"},
    {util::enums::TAccounts::REV, "Revenue"},
    {util::enums::TAccounts::EXPENSE, "Expense"}
};

std::vector<TAccounts> util::enums::assetsTAccounts = {
    util::enums::TAccounts::CASH,
    util::enums::TAccounts::EQUIPMENT,
    util::enums::TAccounts::ACCTRCV,
    util::enums::TAccounts::LAND
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
    util::enums::TAccounts::EXPENSE
};

std::string util::enums::getName(util::enums::TAccounts tAccount){
    return tAccountsNameMap[tAccount];
}