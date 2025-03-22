#include "accounting/accounting_system.hpp"

using namespace accounting;

std::unique_ptr<AccountingSystem> AccountingSystem::instance = NULL;

AccountingSystem::AccountingSystem()
{
    this->transactions = std::make_unique<util::baseclass::AccountingTransactionCollection>();
    this->t_accounts = std::make_unique<util::baseclass::TAccountCollection>();
}

AccountingSystem::~AccountingSystem()
{
#ifdef DEBUG
    std::cout << "Deleting Accounting System" << std::endl;
#endif
}

void AccountingSystem::add_existing_transaction(Transaction *transaction)
{
    this->add_transaction(transaction);
}

void AccountingSystem::add_entry(std::shared_ptr<Entry> entry)
{
    std::unique_ptr<util::baseclass::HasTable> item_from_db = NULL;
    try
    {
        item_from_db = this->t_accounts->get_from_database(entry->get_t_account());
    }
    catch (const std::exception &e)
    {
        item_from_db = std::make_unique<accounting::TAccount>(entry->get_t_account());
        this->t_accounts->insert_new_item(item_from_db.get());
    }
    std::unique_ptr<accounting::TAccount> t_account_from_db((accounting::TAccount *)item_from_db.release());
    t_account_from_db->add_entry(entry.get());
    this->t_accounts->update_existing_item(t_account_from_db.get());
}

AccountingSystem *AccountingSystem::get_instance()
{
    if (AccountingSystem::instance.get() == NULL)
    {
        AccountingSystem::instance.reset(new AccountingSystem());
    }
    return AccountingSystem::instance.get();
}

void AccountingSystem::add_transaction(Transaction *transaction)
{
    this->transactions->insert_new_item(transaction);
    for (std::shared_ptr<Entry> entry : transaction->get_debit_entries())
    {
        this->add_entry(entry);
    }
    for (std::shared_ptr<Entry> entry : transaction->get_credit_entries())
    {
        this->add_entry(entry);
    }
}

void AccountingSystem::end_year_adjustment()
{
    std::unique_ptr<util::Date> now = std::make_unique<util::Date>();
    std::string transaction_title = "Closing the book";
    std::vector<std::unique_ptr<util::baseclass::HasTable>> items = this->t_accounts->get_temporary_accounts();
    std::vector<TAccount *> temporary_accounts;
    for (std::unique_ptr<util::baseclass::HasTable> &item : items)
    {
        temporary_accounts.push_back((TAccount *)item.get());
    }
    std::unique_ptr<accounting::Transaction> close_the_book =
        util::factory::ClosingTemporaryAccountsFactory(std::move(now), transaction_title, temporary_accounts)
            .create_transaction();
    this->add_transaction(close_the_book.get());
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