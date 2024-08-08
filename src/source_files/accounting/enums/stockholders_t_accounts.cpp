#include "accounting/enums/stockholders_t_accounts.hpp"

using namespace accounting;

std::map<enums::StockholdersTAccount, std::string> enums::stockholdersTAccountNames = {
    {enums::COMMONSTOCK, "Common Stock"},
    {enums::ADDTLCPT, "Additional Paid-in Capital"},
    {enums::REV, "Revenue"},
    {enums::EXPENSE, "Expense"}};

enums::StockholdersTAccount enums::allStockholdersTAccounts[] = {enums::COMMONSTOCK, enums::ADDTLCPT, enums::REV, enums::EXPENSE};

int enums::asInt(StockholdersTAccount tAccount)
{
    return static_cast<int>(tAccount);
}