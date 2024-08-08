#include "accounting/enums/liabilities_t_accounts.hpp"

using namespace accounting;

std::map<enums::LiabilitiesTAccount, std::string> enums::liabilitiesTAccountNames = {
    {enums::ACCPAYABLE, "Account Payable"},
    {enums::NOTEPAYABLE, "Notes Payable"},
    {enums::UNEARNEDREV, "Unearned Revenue"}
    };

enums::LiabilitiesTAccount enums::allLiabilitiesTAccounts[] = {enums::ACCPAYABLE, enums::NOTEPAYABLE, enums::UNEARNEDREV};

int enums::asInt(LiabilitiesTAccount tAccount)
{
    return static_cast<int>(tAccount);
}