#include "accounting/accounts/t_account.hpp"
using namespace accounting;

TAccount::TAccount(util::enums::TAccounts title, util::enums::AccountTitles account_title, double debit, double credit)
    : util::baseclass::HasTable(util::enums::PrimaryKeyPrefix::NOKEY)
{
    this->title = title;
    this->account_title = account_title;
    this->debit_amount = debit;
    this->credit_amount = credit;
    this->set_db_code(util::enums::t_accounts_name_map[this->title]);
}

TAccount::TAccount(util::enums::TAccounts title)
    : TAccount(title, util::enums::t_accounts_acc_title_map[title], 0, 0)
{
}

TAccount::~TAccount()
{
}

util::enums::TAccounts TAccount::get_title()
{
    return this->title;
}

std::string TAccount::get_title_name()
{
    return util::enums::t_accounts_name_map[this->title];
}

void TAccount::add_entry(Entry *entry)
{
    if (entry->is_debit())
    {
        this->debit_amount += entry->get_amount();
    }
    else
    {
        this->credit_amount += entry->get_amount();
    }
    // this->update_to_db();
}

double TAccount::get_debit_amount()
{  
    return this->debit_amount;
}

double TAccount::get_credit_amount()
{
    return this->credit_amount;
}

double TAccount::get_total_amount()
{
    return this->get_debit_amount() - this->get_credit_amount();
}

std::string TAccount::to_string()
{
    std::string to_ret = this->get_title_name() + "\n";
    return to_ret;
}