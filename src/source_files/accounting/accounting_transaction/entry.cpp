#include "accounting/accounting_transaction/entry.hpp"
using namespace accounting;

Entry::Entry(std::string db_code,
             std::string transaction_db_code,
             bool debit,
             double amount,
             util::enums::TAccounts t_account)
    : util::baseclass::HasTable(util::enums::PrimaryKeyPrefix::ACCOUNTINGENTRY)
{
    this->set_db_code(db_code);
    this->debit = debit;
    this->amount = amount;
    this->t_account = t_account;
    this->account = util::enums::get_account_title(this->t_account);
    this->transaction_title = "";
    this->transaction_db = transaction_db_code;
}

Entry::Entry(std::string transaction_db_code, bool debit, double amount, util::enums::TAccounts t_account)
    : Entry::Entry("", transaction_db_code, debit, amount, t_account)
{
}

Entry::~Entry()
{
#ifdef DEBUG
    std::cout << "Deleting Accounting Entry" << std::endl;
#endif
}

bool Entry::is_debit()
{
    return this->debit;
}

double Entry::get_amount()
{
    return this->amount;
}

std::string Entry::get_transaction_db()
{
    return this->transaction_db;
}

std::string Entry::get_transaction_title()
{
    return this->transaction_title;
}

util::enums::TAccounts Entry::get_t_account()
{
    return this->t_account;
}

util::enums::AccountTitles Entry::get_account_title()
{
    return this->account;
}

std::string Entry::get_t_account_name()
{
    return util::enums::get_name(this->t_account);
}

std::string Entry::get_account_title_name()
{
    return util::enums::get_name(this->account);
}

void Entry::set_transaction_db(std::string transaction_db)
{
    this->transaction_db = transaction_db;
}

void Entry::set_transaction_title(std::string title)
{
    this->transaction_title = title;
}

std::string Entry::to_string()
{
    std::string to_ret = "";
    if (this->is_debit())
    {
        to_ret += this->get_db_code() + " D \"" + this->get_transaction_title() + "\" " + std::to_string(this->amount);
    }
    else
    {
        to_ret += this->get_db_code() + " C \"" + this->get_transaction_title() + "\" " + std::to_string(this->amount);
    }
    to_ret += "\n";
    return to_ret;
}