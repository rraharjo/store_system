#include "accounting/accounts/account.hpp"
#ifdef _WIN32
using namespace accounting;

void Account::add_t_account(util::enums::TAccounts t_account)
{
    //this->t_accounts[t_account] = TAccount::generate_from_database(t_account);
}

Account::Account(bool debit, util::enums::AccountTitles title)
{
    this->debit = debit;
    this->title = title;
    this->t_accounts = {};
}

Account::~Account()
{
    for (auto it = this->t_accounts.begin(); it != this->t_accounts.end(); it++)
    {
        delete it->second;
    }
}

util::enums::AccountTitles Account::get_title()
{
    return this->title;
}

std::string Account::get_title_name()
{
    return util::enums::get_name(this->title);
}

double Account::get_total_credit()
{
    double to_ret = 0.0;
    for (auto it = this->t_accounts.begin(); it != this->t_accounts.end(); it++)
    {
        to_ret += it->second->get_credit_amount();
    }
    return to_ret;
}

double Account::get_total_debit()
{
    double to_ret = 0.0;
    for (auto it = this->t_accounts.begin(); it != this->t_accounts.end(); it++)
    {
        to_ret += it->second->get_debit_amount();
    }
    return to_ret;
}

void Account::add_entry(Entry *entry)
{
    this->t_accounts[entry->get_t_account()]->add_entry(entry);
}

std::string Account::to_string()
{
    std::string to_ret = this->get_title_name();
    to_ret += "\n";
    int num = 1;
    for (auto it = this->t_accounts.begin(); it != this->t_accounts.end(); it++)
    {
        to_ret += std::to_string(num++) + ". " + it->second->to_string();
    }
    return to_ret;
}

Assets::Assets() : Account(true, util::enums::AccountTitles::ASSETS)
{
    this->initiate_t_account();
}

void Assets::initiate_t_account()
{
    for (util::enums::TAccounts t_account : util::enums::assets_t_accounts)
    {
        this->add_t_account(t_account);
    }
}

Liabilities::Liabilities() : Account(false, util::enums::AccountTitles::LIABILITIES)
{
    this->initiate_t_account();
}

void Liabilities::initiate_t_account()
{
    for (util::enums::TAccounts t_account : util::enums::liabilities_t_accounts)
    {
        this->add_t_account(t_account);
    }
}

StockholdersEquityAccount::StockholdersEquityAccount() : Account(false, util::enums::AccountTitles::STOCKHOLDERSEQUITY)
{
    this->initiate_t_account();
}

void StockholdersEquityAccount::initiate_t_account()
{
    for (util::enums::TAccounts t_account : util::enums::stockholders_t_accounts)
    {
        this->add_t_account(t_account);
    }
}

std::vector<TAccount *> StockholdersEquityAccount::get_temporary_accounts()
{
    std::vector<TAccount *> t_accounts;
    for (util::enums::TAccounts current_account = MIN_TEMPORARY_ACCOUNT;
         current_account < MAX_TEMPORARY_ACCOUNT; ++current_account)
    {
        t_accounts.push_back(this->t_accounts[current_account]);
    }
    return t_accounts;
}
#endif