#include "util/enums/accounting/liabilities_t_accounts.hpp"

using namespace util::enums;

std::map<LiabilitiesTAccount, std::string> util::enums::liabilitiesTAccountNames = {
    {ACCPAYABLE, "Account Payable"},
    {NOTEPAYABLE, "Notes Payable"},
    {UNEARNEDREV, "Unearned Revenue"}
    };

LiabilitiesTAccount util::enums::allLiabilitiesTAccounts[] = {ACCPAYABLE, NOTEPAYABLE, UNEARNEDREV};

int util::enums::asInt(LiabilitiesTAccount tAccount)
{
    return static_cast<int>(tAccount);
}