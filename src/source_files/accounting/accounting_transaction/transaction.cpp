#include "accounting/accounting_transaction/transaction.hpp"
using namespace accounting;

Transaction::Transaction(std::string db_code, std::string name, std::unique_ptr<util::Date> transaction_date, std::string pid)
    : util::baseclass::HasTable(util::enums::PrimaryKeyPrefix::ACCOUNTINGTRANSACTION)
{
    this->set_db_code(db_code);
    this->name = name;
    this->debit_entries = {};
    this->credit_entries = {};
    this->transaction_date = std::move(transaction_date);
    this->entity_id = pid;
}

Transaction::Transaction(std::string name, std::unique_ptr<util::Date> transaction_date, std::string pid)
    : util::baseclass::HasTable(util::enums::PrimaryKeyPrefix::ACCOUNTINGTRANSACTION)
{
    this->name = name;
    this->debit_entries = {};
    this->credit_entries = {};
    this->transaction_date = std::move(transaction_date);
    this->entity_id = pid;
}

Transaction::Transaction(std::string name, std::unique_ptr<util::Date> transaction_date)
    : Transaction::Transaction(name, std::move(transaction_date), "")
{
}

Transaction::Transaction(std::string name, std::string pid)
    : Transaction::Transaction(name, std::make_unique<util::Date>(), pid)
{
}

Transaction::Transaction(std::string name) : Transaction::Transaction(name, std::make_unique<util::Date>())
{
}

Transaction::~Transaction()
{
#ifdef DEBUG
    std::cout << "Deleting Accounting Transaction" << std::endl;
#endif
}

std::string Transaction::get_name()
{
    return this->name;
}

std::string Transaction::get_entity_id()
{
    return this->entity_id;
}

util::Date *Transaction::get_transaction_date()
{
    return this->transaction_date.get();
}

std::vector<std::shared_ptr<Entry>> Transaction::get_debit_entries()
{
    return this->debit_entries;
}

std::vector<std::shared_ptr<Entry>> Transaction::get_credit_entries()
{
    return this->credit_entries;
}

double Transaction::get_debit_amount()
{
    double total = 0;
    for (std::shared_ptr<Entry> entry : this->debit_entries)
    {
        total += entry.get()->get_amount();
    }
    return total;
}

double Transaction::get_credit_amount()
{
    double total = 0;
    for (std::shared_ptr<Entry> entry : this->credit_entries)
    {
        total += entry.get()->get_amount();
    }
    return total;
}

void Transaction::add_entry(std::unique_ptr<Entry> entry)
{
    entry->set_transaction_db(this->get_db_code());
    entry->set_transaction_title(this->name);
    if (entry->is_debit())
    {
        std::shared_ptr<Entry> to_add;
        to_add.reset(entry.release());
        this->debit_entries.push_back(to_add);
    }
    else
    {
        std::shared_ptr<Entry> to_add;
        to_add.reset(entry.release());
        this->credit_entries.push_back(to_add);
    }
}

bool Transaction::is_balanced()
{
    return this->get_debit_amount() == this->get_credit_amount();
}

std::string Transaction::to_string()
{
    std::string to_ret = "";
    to_ret += "database code: " + this->get_db_code() + "\n";
    to_ret += "name: " + this->name + "\n";
    to_ret += "date: " + this->transaction_date->to_string() + "\n";
    to_ret += "related entity: " + this->entity_id + "\n";
    to_ret += "=========Entries=========\n";
    for (std::shared_ptr<Entry> entry : this->debit_entries)
    {
        to_ret += entry.get()->to_string();
    }
    for (std::shared_ptr<Entry> entry : this->credit_entries)
    {
        to_ret += entry.get()->to_string();
    }
    return to_ret;
}
