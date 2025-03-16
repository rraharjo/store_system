#include "accounting/accounting_system.hpp"

using namespace accounting;
AccountingSystem::AccountingSystem()
{
    this->transactions = std::unique_ptr<util::baseclass::AccountingTransactionCollection>(new util::baseclass::AccountingTransactionCollection());
    this->t_accounts = std::unique_ptr<util::baseclass::TAccountCollection>(new util::baseclass::TAccountCollection());
    // this->assets = new Assets();
    // this->liabilities = new Liabilities();
    // this->stockholders_equity = new StockholdersEquityAccount();
}

AccountingSystem::~AccountingSystem()
{
    // delete this->assets;
    // delete this->liabilities;
    // delete this->stockholders_equity;
}

void AccountingSystem::add_existing_transaction(Transaction *transaction)
{
    this->add_transaction(transaction);
}

void AccountingSystem::add_entry(Entry *entry)
{
    util::baseclass::HasTable *item_from_db = NULL;
    try
    {
        item_from_db = this->t_accounts->get_from_database(entry->get_t_account());
    }
    catch (const std::exception &e)
    {
        item_from_db = new accounting::TAccount(entry->get_t_account());
        this->t_accounts->insert_new_item(item_from_db);
    }
    accounting::TAccount *t_account_from_db = (accounting::TAccount *)item_from_db;
    t_account_from_db->add_entry(entry);
    this->t_accounts->update_existing_item(t_account_from_db);
    delete t_account_from_db;
    // switch (entry->get_account_title())
    // {
    // case util::enums::AccountTitles::ASSETS:
    //     this->assets->add_entry(entry);
    //     break;
    // case util::enums::AccountTitles::LIABILITIES:
    //     this->liabilities->add_entry(entry);
    //     break;
    // case util::enums::AccountTitles::STOCKHOLDERSEQUITY:
    //     this->stockholders_equity->add_entry(entry);
    //     break;
    // }
}

AccountingSystem *AccountingSystem::get_instance()
{
    if (AccountingSystem::instance == NULL)
    {
        // TAccount::initiate_t_account_on_db();
        AccountingSystem::instance = new AccountingSystem();
    }
    return AccountingSystem::instance;
}

void AccountingSystem::add_transaction(Transaction *transaction)
{
    this->transactions->insert_new_item(transaction);
    for (Entry *entry : transaction->get_debit_entries())
    {
        this->add_entry(entry);
    }
    for (Entry *entry : transaction->get_credit_entries())
    {
        this->add_entry(entry);
    }
}

void AccountingSystem::end_year_adjustment() // TO DO: adjust the closing the book entries for all temporary t-accounts
{
    util::Date *now = new util::Date();
    std::string transaction_title = "Closing the book";
    std::vector<util::baseclass::HasTable *> items = this->t_accounts->get_temporary_accounts();
    std::vector<TAccount *> temporary_accounts;
    for (util::baseclass::HasTable *item : items)
    {
        temporary_accounts.push_back((TAccount *)item);
    }
    accounting::Transaction *close_the_book =
        util::factory::ClosingTemporaryAccountsFactory(now, transaction_title, temporary_accounts)
            .create_transaction();
    this->add_transaction(close_the_book);
}

std::string AccountingSystem::to_string()
{
    std::string to_ret = "";
    // to_ret += this->assets->to_string();
    // to_ret += "\n";
    // to_ret += this->liabilities->to_string();
    // to_ret += "\n";
    // to_ret += this->stockholders_equity->to_string();
    return to_ret;
}

AccountingSystem *AccountingSystem::instance = NULL;