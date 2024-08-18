#include "util/enums/accounting/t_accounts.hpp"

using namespace util::enums;

std::map<TAccounts, std::string> util::enums::tAccountsNameMap = {
    //assets
    {CASH, "Cash"},
    {EQUIPMENT, "Equipment"},
    {ACCTRCV, "Account Receivable"},
    {LAND, "Land"},
    //liabilities
    {ACCPAYABLE, "Account Payable"},
    {NOTEPAYABLE, "Notes Payable"},
    {UNEARNEDREV, "Unearned Revenue"},
    //stockholders equity
    {COMMONSTOCK, "Common Stock"},
    {ADDTLCPT, "Additional Paid-in Capital"},
    {REV, "Revenue"},
    {EXPENSE, "Expense"}
};

std::vector<TAccounts> util::enums::assetsTAccounts = {
    CASH,
    EQUIPMENT,
    ACCTRCV,
    LAND
};

std::vector<TAccounts> util::enums::liabilitiesTAccounts = {
    ACCPAYABLE,
    NOTEPAYABLE,
    UNEARNEDREV
};

std::vector<TAccounts> util::enums::stockholdersTAccounts = {
    COMMONSTOCK,
    ADDTLCPT,
    REV,
    EXPENSE
};