#include "accounting/accounting_transaction/transaction.hpp"
using namespace accounting;

util::Table *Transaction::class_table = util::AccountingTransactionTable::get_instance();

std::vector<Transaction *> Transaction::generate_from_database()
{
    util::Date now = util::Date();
    std::string string_date = std::to_string(now.get_m_day()) + "/" +
                             std::to_string(now.get_month()) + "/" +
                             std::to_string(now.get_year() - 1);
    util::Date last_year = util::Date(string_date);
    std::vector<util::TableCondition> conditions;
    util::TableCondition cond1 = util::TableCondition();
    cond1.col = util::enums::accounting_transaction_table_columns[util::enums::AccountingTransactionTable::TRANSACTIONDATE];
    cond1.comparator = util::TableComparator::MORETHAN;
    cond1.value = last_year.to_db_format();
    conditions.push_back(cond1);
    std::vector<Transaction *> to_ret;
    std::vector<std::vector<std::string>> records = class_table->get_records(conditions);
    for (std::vector<std::string> &record : records)
    {
        util::Date *transaction_date = new util::Date(record[2], "%Y-%m-%d");
        Transaction *transaction_from_db = new Transaction(record[0], record[1], transaction_date, record[3]);
        std::vector<Entry *> entries = Entry::generate_from_db(transaction_from_db->get_db_code());
        for (Entry *entry : entries){
            transaction_from_db->add_entry(entry);
        }
        to_ret.push_back(transaction_from_db);
    }
    return to_ret;
}

void Transaction::insert_to_db()
{
    this->insert_to_db_with_table(Transaction::class_table);
};

void Transaction::update_to_db()
{
    this->update_to_db_with_table(Transaction::class_table);
};

Transaction::Transaction(std::string db_code, std::string name, util::Date *transaction_date, std::string pid)
    : util::baseclass::HasTable()
{
    this->set_db_code(db_code);
    this->name = name;
    this->debit_entries = {};
    this->credit_entries = {};
    this->transaction_date = transaction_date;
    this->entity_id = pid;
}

Transaction::Transaction(std::string name, util::Date *transaction_date, std::string pid)
    : util::baseclass::HasTable()
{
    this->name = name;
    this->debit_entries = {};
    this->credit_entries = {};
    this->transaction_date = transaction_date;
    this->entity_id = pid;
}

Transaction::Transaction(std::string name, util::Date *transaction_date)
    : Transaction::Transaction(name, transaction_date, "")
{
}

Transaction::Transaction(std::string name, std::string pid)
    : Transaction::Transaction(name, new util::Date(), pid)
{
}

Transaction::Transaction(std::string name) : Transaction::Transaction(name, new util::Date())
{
}

Transaction::~Transaction()
{
    for (Entry *e : this->credit_entries)
    {
        if (e->get_db_code() != "")
        {
            delete e;
        }
    }

    for (Entry *e : this->debit_entries)
    {
        if (e->get_db_code() != "")
        {
            delete e;
        }
    }
}

std::vector<std::string> Transaction::get_insert_parameter()
{
    std::vector<std::string> args;
    args.push_back(util::enums::primary_key_codes_map[util::enums::PrimaryKeyCodes::ACCOUNTINGTRANSACTION]);
    args.push_back(this->name);
    args.push_back(this->transaction_date->to_db_format());
    args.push_back(this->entity_id == "" ? "NULL" : this->entity_id);
    return args;
}

std::vector<std::string> Transaction::get_update_parameter()
{
    std::vector<std::string> args;
    args.push_back(this->name);
    args.push_back(this->transaction_date->to_db_format());
    args.push_back(this->entity_id == "" ? "NULL" : this->entity_id);
    return args;
}

std::vector<Entry *> &Transaction::get_debit_entries()
{
    return this->debit_entries;
}

std::vector<Entry *> &Transaction::get_credit_entries()
{
    return this->credit_entries;
}

double Transaction::get_debit_amount()
{
    double total = 0;
    for (Entry *entry : this->debit_entries)
    {
        total += (*entry).get_amount();
    }
    return total;
}

double Transaction::get_credit_amount()
{
    double total = 0;
    for (Entry *entry : this->credit_entries)
    {
        total += (*entry).get_amount();
    }
    return total;
}

void Transaction::add_entry(Entry *entry)
{
    if (entry->is_debit())
    {
        this->debit_entries.push_back(entry);
    }
    else
    {
        this->credit_entries.push_back(entry);
    }
    entry->set_transaction_db(this->get_db_code());
    entry->set_transaction_title(this->name);
}

bool Transaction::is_balanced()
{
    return this->get_debit_amount() == this->get_credit_amount();
}

std::string Transaction::to_string(){
    std::string to_ret = "";
    to_ret += "database code: " + this->get_db_code() + "\n";
    to_ret += "name: " + this->name + "\n";
    to_ret += "date: " + this->transaction_date->to_string() + "\n";
    to_ret += "related entity: " + this->entity_id + "\n";
    to_ret += "=========Entries=========\n";
    for (Entry *entry : this->debit_entries){
        to_ret += entry->to_string();
    }
    for (Entry *entry : this->credit_entries){
        to_ret += entry->to_string();
    }
    return to_ret;
}
