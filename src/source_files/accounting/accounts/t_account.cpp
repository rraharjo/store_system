#include "accounting/accounts/t_account.hpp"
using namespace accounting;

util::Table *TAccount::class_table = util::TAccountTable::get_instance();

void TAccount::initiate_t_account_on_db()
{
    std::vector<TAccount *> t_account_collections;
    for (util::enums::TAccounts current = FIRST_TACCOUNT; current != LAST_TACCOUNT; ++current)
    {
        TAccount *new_t_account = new TAccount(current);
        new_t_account->insert_to_db();
        t_account_collections.push_back(new_t_account);
    }
    for (TAccount *t_account : t_account_collections)
    {
        delete t_account;
    }
}

TAccount *TAccount::generate_from_database(util::enums::TAccounts t_account)
{
    std::vector<util::TableCondition> conditions;
    std::vector<std::vector<std::string>> records;
    util::TableCondition matching_title = util::TableCondition();
    matching_title.col = util::enums::t_account_table_columns[util::enums::TAccountTable::TITLE];
    matching_title.comparator = util::TableComparator::EQUAL;
    matching_title.value = util::enums::t_accounts_name_map[t_account];
    conditions.push_back(matching_title);
    records = TAccount::class_table->get_records(conditions);
    return new TAccount(t_account, std::stod(records[0][1]), std::stod(records[0][2]));
}

std::vector<std::string> TAccount::get_insert_parameter()
{
    std::vector<std::string> to_ret;
    to_ret.push_back(util::enums::t_accounts_name_map[this->title]);
    to_ret.push_back(std::to_string(this->debit_amount));
    to_ret.push_back(std::to_string(this->credit_amount));
    return to_ret;
}

std::vector<std::string> TAccount::get_update_parameter()
{
    std::vector<std::string> to_ret;
    to_ret.push_back(this->get_db_code());
    to_ret.push_back(std::to_string(this->debit_amount));
    to_ret.push_back(std::to_string(this->credit_amount));
    return to_ret;
}

void TAccount::insert_to_db()
{
    this->insert_to_db_with_table(TAccount::class_table, false);
};

void TAccount::update_to_db()
{
    this->update_to_db_with_table(TAccount::class_table);
};

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
    this->update_to_db();
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