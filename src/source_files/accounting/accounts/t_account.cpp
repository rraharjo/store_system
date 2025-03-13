#include "accounting/accounts/t_account.hpp"
using namespace accounting;

TAccount::TAccount(util::enums::TAccounts title, double debit, double credit)
    : util::baseclass::HasTable()
{
    this->title = title;
    this->debit_amount = debit;
    this->credit_amount = credit;
    this->set_db_code(util::enums::t_accounts_name_map[this->title]);
    this->debit_entries = std::vector<Entry *>();
    this->credit_entries = std::vector<Entry *>();
}

TAccount::TAccount(util::enums::TAccounts title)
    : TAccount(title, 0, 0)
{
}

TAccount::~TAccount()
{
    for (Entry *e : this->debit_entries)
    {
        delete e;
    }
    for (Entry *e : this->credit_entries)
    {
        delete e;
    }
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
        this->debit_entries.push_back(entry);
        this->debit_amount += entry->get_amount();
    }
    else
    {
        this->credit_entries.push_back(entry);
        this->credit_amount += entry->get_amount();
    }
    //this->update_to_db();
}

double TAccount::get_debit_amount()
{
    double total = 0;
    for (Entry *event : this->debit_entries)
    {
        total += event->get_amount();
    }
    return total;
}

double TAccount::get_credit_amount()
{
    double total = 0;
    for (Entry *event : this->credit_entries)
    {
        total += event->get_amount();
    }
    return total;
}

double TAccount::get_total_amount()
{
    return this->get_debit_amount() - this->get_credit_amount();
}

std::string TAccount::to_string()
{
    std::string to_ret = this->get_title_name() + "\n";
    for (Entry *entry : this->debit_entries)
    {
        to_ret += entry->to_string();
    }
    for (Entry *entry : this->credit_entries)
    {
        to_ret += entry->to_string();
    }
    return to_ret;
}