#include "accounting/accounting_system.hpp"

using namespace accounting;
AccountingSystem::AccountingSystem()
{
    this->assets = new Assets();
    this->liabilities = new Liabilities();
    this->stockholders_equity = new StockholdersEquityAccount();
}

AccountingSystem::~AccountingSystem()
{
    delete this->assets;
    delete this->liabilities;
    delete this->stockholders_equity;
}

void AccountingSystem::add_existing_transaction(Transaction *transaction)
{
    this->add_transaction(transaction);
}

void AccountingSystem::add_entry(Entry *entry)
{
    switch (entry->get_account_title())
    {
    case util::enums::AccountTitles::ASSETS:
        this->assets->add_entry(entry);
        break;
    case util::enums::AccountTitles::LIABILITIES:
        this->liabilities->add_entry(entry);
        break;
    case util::enums::AccountTitles::STOCKHOLDERSEQUITY:
        this->stockholders_equity->add_entry(entry);
        break;
    }
}

AccountingSystem *AccountingSystem::get_instance()
{
    if (AccountingSystem::instance == NULL)
    {
        TAccount::initiate_t_account_on_db();
        AccountingSystem::instance = new AccountingSystem();
    }
    return AccountingSystem::instance;
}

void AccountingSystem::add_transaction(Transaction *transaction)
{
    for (Entry *entry : transaction->get_debit_entries())
    {
        this->add_entry(entry);
    }
    for (Entry *entry : transaction->get_credit_entries())
    {
        this->add_entry(entry);
    }
    this->transactions[transaction->get_db_code()] = transaction;
}

void AccountingSystem::end_year_adjustment()//TO DO: adjust the closing the book entries for all temporary t-accounts
{
    util::Date *now = new util::Date();
    std::string transaction_title = "Closing the book";
    std::vector<TAccount *> temporary_accounts = this->stockholders_equity->get_temporary_accounts();
    accounting::Transaction *close_the_book =
        util::factory::ClosingTemporaryAccountsFactory(now, transaction_title, temporary_accounts)
            .create_transaction();
    this->add_transaction(close_the_book);
}

std::string AccountingSystem::to_string()
{
    std::string to_ret = "";
    to_ret += this->assets->to_string();
    to_ret += "\n";
    to_ret += this->liabilities->to_string();
    to_ret += "\n";
    to_ret += this->stockholders_equity->to_string();
    return to_ret;
}

AccountingSystem *AccountingSystem::instance = NULL;