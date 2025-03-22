#include "util/class/accounting_entries_collection.hpp"

namespace util
{
    namespace baseclass
    {
        AccountingEntryCollection::AccountingEntryCollection()
            : Collection(
                  util::enums::PrimaryKeyPrefix::ACCOUNTINGENTRY,
                  util::AccountingEntryTable::get_instance())
        {
        }

        AccountingEntryCollection::~AccountingEntryCollection()
        {
#ifdef DEBUG
            std::cout << "Deleting Accounting Entry Collection" << std::endl;
#endif
        }

        void AccountingEntryCollection::insert_new_item(HasTable *new_item)
        {
            Collection::validate_insert(new_item);
            accounting::Entry *new_entry = (accounting::Entry *)new_item;
            std::vector<std::string> parameter = {
                util::enums::primary_key_prefix_map[this->primary_key_prefix],
                new_entry->get_transaction_db(),
                new_entry->is_debit() ? "true" : "false",
                std::to_string(new_entry->get_amount()),
                util::enums::t_accounts_name_map[new_entry->get_t_account()],
                // util::enums::account_titles_map[new_entry->get_account_title()]
            };
            std::vector<std::string> res = this->table->insert_row(parameter);
            Collection::set_db_code(new_entry, res[0]);
        }

        void AccountingEntryCollection::update_existing_item(HasTable *existing_item)
        {
            Collection::validate_update(existing_item);
            accounting::Entry *existing_entry = (accounting::Entry *)existing_item;
            std::vector<std::string> parameter = {
                existing_entry->get_transaction_db(),
                existing_entry->is_debit() ? "true" : "false",
                std::to_string(existing_entry->get_amount()),
                util::enums::t_accounts_name_map[existing_entry->get_t_account()],
                // util::enums::account_titles_map[new_entry->get_account_title()]
            };
            this->table->update_row(existing_entry->get_db_code(), parameter);
        }

        std::unique_ptr<HasTable> AccountingEntryCollection::get_from_database(std::string db_code)
        {
            if (db_code.rfind(util::enums::primary_key_prefix_map[this->primary_key_prefix]) != 0)
            {
                throw std::invalid_argument("Cannot get a " + db_code + " from " +
                                            util::enums::primary_key_prefix_map[this->primary_key_prefix] + " table...\n");
            }
            std::vector<util::TableCondition> conditions;
            util::TableCondition equal_db_code;
            equal_db_code.col = util::enums::accounting_entry_table_columns[util::enums::AccountingEntryTable::DATABASECODE];
            equal_db_code.comparator = TableComparator::EQUAL;
            equal_db_code.value = db_code;
            conditions.push_back(equal_db_code);
            std::vector<std::vector<std::string>> records = this->table->get_records(conditions);
            if (records.empty())
            {
                throw std::invalid_argument("No item with code " + db_code + " in the database");
            }
            std::vector<std::string> record = records[0];
            std::unique_ptr<accounting::Entry> new_entry =
                std::make_unique<accounting::Entry>(record[0],
                                                    record[1],
                                                    record[2] == "t" ? true : false,
                                                    std::stod(record[3]),
                                                    util::enums::get_t_account_enum(record[4]));
            std::unique_ptr<HasTable> to_ret((HasTable *)new_entry.release());
            return to_ret;
        }

        std::vector<std::unique_ptr<HasTable>> AccountingEntryCollection::get_from_database(std::vector<util::TableCondition> &conditions)
        {
            std::vector<std::unique_ptr<HasTable>> to_ret;
            std::vector<std::vector<std::string>> records = this->table->get_records(conditions);
            for (std::vector<std::string> &record : records)
            {
                std::unique_ptr<accounting::Entry> new_entry =
                    std::make_unique<accounting::Entry>(record[0],
                                                        record[1],
                                                        record[2] == "t" ? true : false,
                                                        std::stod(record[3]),
                                                        util::enums::get_t_account_enum(record[4]));
                std::unique_ptr<HasTable> to_add((HasTable *)new_entry.release());
                to_ret.push_back(std::move(to_add));
            }
            return to_ret;
        }
    }
}