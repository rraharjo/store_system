#include "util/class/t_account_collection.hpp"

namespace util
{
    namespace baseclass
    {
        TAccountCollection::TAccountCollection() : Collection(util::enums::primary_key_codes_map[util::enums::PrimaryKeyCodes::NOKEY],
                                                              util::TAccountTable::get_instance())
        {
        }

        void TAccountCollection::insert_new_item(HasTable *new_item)
        {
            std::vector<util::TableCondition> conditions;
            accounting::TAccount *t_acc = (accounting::TAccount *)new_item;
            util::TableCondition condition;
            condition.col = util::enums::t_account_table_columns[util::enums::TAccountTable::TITLE];
            condition.comparator = util::TableComparator::EQUAL;
            condition.value = util::enums::t_accounts_name_map[t_acc->get_title()];
            conditions.push_back(condition);
            std::vector<std::vector<std::string>> res = this->table->get_records(conditions);
            if (res.size() == 0)
            {
                std::vector<std::string> values = {
                    util::enums::t_accounts_name_map[t_acc->get_title()],
                    "0",
                    "0"};
                this->table->insert_row(values);
            }
        }

        void TAccountCollection::update_existing_item(HasTable *existing_item)
        {
            accounting::TAccount *t_acc = (accounting::TAccount *)existing_item;
            std::vector<std::string> values = {
                //t_acc->get_db_code(),
                t_acc->get_title_name(),
                std::to_string(t_acc->get_debit_amount()),
                std::to_string(t_acc->get_credit_amount()),
            };
            this->table->update_row(t_acc->get_title_name(), values);
        }

        HasTable *TAccountCollection::get_from_database(std::string db_code)
        {
            throw std::invalid_argument("use get_from_database with util::enums::TAccounts paremeter");
        }

        HasTable *TAccountCollection::get_from_database(util::enums::TAccounts title)
        {
            std::string t_account_name = util::enums::t_accounts_name_map[title];
            std::vector<util::TableCondition> conditions;
            util::TableCondition equal_db_code;
            equal_db_code.col = util::enums::t_account_table_columns[util::enums::TAccountTable::TITLE];
            equal_db_code.comparator = TableComparator::EQUAL;
            equal_db_code.value = t_account_name;
            conditions.push_back(equal_db_code);
            std::vector<std::vector<std::string>> records = this->table->get_records(conditions);
            if (records.empty())
            {
                throw std::invalid_argument("No T-Account with title " + t_account_name + " in the database...\n");
            }
            std::vector<std::string> record = records[0];
            util::baseclass::HasTable *t_acc_from_db = new accounting::TAccount(title, std::stod(record[2]), std::stod(record[3]));
            return t_acc_from_db;
        }
    };
}