#include "util/enums/accounting/stockholders_t_accounts.hpp"

using namespace util::enums;

std::map<StockholdersTAccount, std::string> util::enums::stockholdersTAccountNames = {
    {COMMONSTOCK, "Common Stock"},
    {ADDTLCPT, "Additional Paid-in Capital"},
    {REV, "Revenue"},
    {EXPENSE, "Expense"}};

StockholdersTAccount util::enums::allStockholdersTAccounts[] = {COMMONSTOCK, ADDTLCPT, REV, EXPENSE};

int util::enums::asInt(StockholdersTAccount tAccount)
{
    return static_cast<int>(tAccount);
}