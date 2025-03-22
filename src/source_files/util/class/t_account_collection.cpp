#include "util/class/t_account_collection.hpp"

namespace util
{
    namespace baseclass
    {
        TAccountCollection::TAccountCollection()
            : Collection(util::enums::PrimaryKeyPrefix::NOKEY,
                         util::TAccountTable::get_instance())
        {
        }

        TAccountCollection::~TAccountCollection()
        {
#ifdef DEBUG
            std::cout << "Deleting TAccount Collection" << std::endl;
#endif
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
                util::enums::AccountTitles this_account_title = util::enums::t_accounts_acc_title_map[t_acc->get_title()];
                std::vector<std::string> values = {
                    util::enums::t_accounts_name_map[t_acc->get_title()],
                    util::enums::account_titles_map[this_account_title],
                    "0",
                    "0"};
                this->table->insert_row(values);
            }
        }

        void TAccountCollection::update_existing_item(HasTable *existing_item)
        {
            accounting::TAccount *t_acc = (accounting::TAccount *)existing_item;
            util::enums::AccountTitles this_account_title = util::enums::t_accounts_acc_title_map[t_acc->get_title()];
            std::vector<std::string> values = {
                // t_acc->get_db_code(),
                t_acc->get_title_name(),
                util::enums::account_titles_map[this_account_title],
                std::to_string(t_acc->get_debit_amount()),
                std::to_string(t_acc->get_credit_amount()),
            };
            this->table->update_row(t_acc->get_title_name(), values);
        }

        std::unique_ptr<HasTable> TAccountCollection::get_from_database(std::string db_code)
        {
            throw std::invalid_argument("use get_from_database with util::enums::TAccounts paremeter");
            std::unique_ptr<HasTable> to_ret;
            return std::move(to_ret);
        }

        std::unique_ptr<HasTable> TAccountCollection::get_from_database(util::enums::TAccounts title)
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
                std::string error_msg = "No T-Account with title " + t_account_name + " in the database...";
                throw std::invalid_argument(error_msg);
            }
            std::vector<std::string> record = records[0];
            std::unique_ptr<accounting::TAccount> t_acc_from_db =
                std::make_unique<accounting::TAccount>(title,
                                                       util::enums::t_accounts_acc_title_map[title],
                                                       std::stod(record[2]),
                                                       std::stod(record[3]));
            std::unique_ptr<HasTable> to_ret((HasTable *)t_acc_from_db.release());
            return std::move(to_ret);
        }

        std::vector<std::unique_ptr<HasTable>> TAccountCollection::get_temporary_accounts()
        {
            std::vector<util::enums::TAccounts> temporary_account = {
                util::enums::TAccounts::REV,
                util::enums::TAccounts::EXPENSE,
                util::enums::TAccounts::COGS,
                util::enums::TAccounts::DEPREXP,
                util::enums::TAccounts::WAGEEXP,
                util::enums::TAccounts::RETAINEDEARNINGS,
            };
            std::vector<util::TableCondition> conditions;
            for (util::enums::TAccounts t_account : temporary_account)
            {
                util::TableCondition this_condition;
                this_condition.col = util::enums::t_account_table_columns[util::enums::TAccountTable::TITLE];
                this_condition.comparator = util::TableComparator::EQUAL;
                this_condition.value = util::enums::t_accounts_name_map[t_account];
                conditions.push_back(this_condition);
            }
            std::vector<std::vector<std::string>> rows = this->table->get_records_or_conditions(conditions);
            std::vector<std::unique_ptr<HasTable>> to_ret;
            for (const std::vector<std::string> &row : rows)
            {
                util::enums::TAccounts this_t_acc = util::enums::t_accounts_name_map_inversed[row[0]];
                std::unique_ptr<accounting::TAccount> t_acc_from_db =
                    std::make_unique<accounting::TAccount>(this_t_acc,
                                                           util::enums::t_accounts_acc_title_map[this_t_acc],
                                                           std::stod(row[2]),
                                                           std::stod(row[3]));
                std::unique_ptr<HasTable> to_add((HasTable *)t_acc_from_db.release());
                to_ret.push_back(std::move(to_add));
            }
            return to_ret;
        }
    };
}