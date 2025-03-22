#include "util/class/accounting_transaction_collection.hpp"

namespace util
{
    namespace baseclass
    {
        AccountingTransactionCollection::AccountingTransactionCollection()
            : Collection(
                  util::enums::PrimaryKeyPrefix::ACCOUNTINGTRANSACTION,
                  util::AccountingTransactionTable::get_instance())
        {
            this->entries_collection = std::unique_ptr<AccountingEntryCollection>(new AccountingEntryCollection());
        }

        AccountingTransactionCollection::~AccountingTransactionCollection()
        {
#ifdef DEBUG
            std::cout << "Deleting Accounting Transaction collection" << std::endl;
#endif
        }

        void AccountingTransactionCollection::insert_new_item(HasTable *new_item)
        {
            Collection::validate_insert(new_item);
            accounting::Transaction *new_transaction = (accounting::Transaction *)new_item;
            std::vector<std::string> parameter = {
                util::enums::primary_key_prefix_map[this->primary_key_prefix],
                new_transaction->get_name(),
                new_transaction->get_transaction_date()->to_db_format(),
                new_transaction->get_entity_id() == "" ? "NULL" : new_transaction->get_entity_id(),
            };
            std::vector<std::string> res = this->table->insert_row(parameter);
            Collection::set_db_code(new_transaction, res[0]);
            for (std::shared_ptr<accounting::Entry> new_entry : new_transaction->get_debit_entries())
            {
                new_entry->set_transaction_db(new_transaction->get_db_code());
                this->entries_collection.get()->insert_new_item(new_entry.get());
            }
            for (std::shared_ptr<accounting::Entry> new_entry : new_transaction->get_credit_entries())
            {
                new_entry->set_transaction_db(new_transaction->get_db_code());
                this->entries_collection.get()->insert_new_item(new_entry.get());
            }
        };

        void AccountingTransactionCollection::update_existing_item(HasTable *existing_item)
        {
            Collection::validate_update(existing_item);
            accounting::Transaction *existing_transaction = (accounting::Transaction *)existing_item;
            std::vector<std::string> parameter = {
                existing_transaction->get_name(),
                existing_transaction->get_transaction_date()->to_db_format(),
                existing_transaction->get_entity_id() == "" ? "NULL" : existing_transaction->get_entity_id(),
            };
            for (std::shared_ptr<accounting::Entry> existing_entry : existing_transaction->get_debit_entries())
            {
                this->entries_collection.get()->insert_new_item(existing_entry.get());
            }
            for (std::shared_ptr<accounting::Entry> existing_entry : existing_transaction->get_credit_entries())
            {
                this->entries_collection.get()->insert_new_item(existing_entry.get());
            }
        };

        std::unique_ptr<HasTable> AccountingTransactionCollection::get_from_database(std::string db_code)
        {
            std::string this_primary_key_prefix_string = util::enums::primary_key_prefix_map[this->primary_key_prefix];
            if (db_code.rfind(this_primary_key_prefix_string) != 0)
            {
                throw std::invalid_argument("Cannot get a " + db_code + " from " + this_primary_key_prefix_string + " table...\n");
            }
            // Generate the transaction
            std::vector<util::TableCondition> conditions;
            util::TableCondition equal_db_code;
            equal_db_code.col = util::enums::accounting_transaction_table_columns[util::enums::AccountingTransactionTable::DATABASECODE];
            equal_db_code.comparator = TableComparator::EQUAL;
            equal_db_code.value = db_code;
            conditions.push_back(equal_db_code);
            std::vector<std::vector<std::string>> records = this->table->get_records(conditions);
            if (records.empty())
            {
                throw std::invalid_argument("No item with code " + db_code + " in the database");
            }
            std::vector<std::string> record = records[0];
            std::unique_ptr<util::Date> transaction_date = std::make_unique<util::Date>(record[2], "%Y-%m-%d");
            accounting::Transaction *transaction_from_db = new accounting::Transaction(record[0],
                                                                                       record[1],
                                                                                       std::move(transaction_date),
                                                                                       record[3]);

            // Generate the entries attached to the transaction
            conditions.clear();
            util::TableCondition equal_at_db_code;
            equal_at_db_code.col = util::enums::accounting_entry_table_columns[util::enums::AccountingEntryTable::ATDBCODE];
            equal_at_db_code.comparator = TableComparator::EQUAL;
            equal_at_db_code.value = transaction_from_db->get_db_code();
            conditions.push_back(equal_at_db_code);
            std::vector<std::unique_ptr<HasTable>> entries = this->entries_collection.get()->get_from_database(conditions);
            for (std::unique_ptr<HasTable> &entry : entries)
            {
                std::unique_ptr<accounting::Entry> to_add((accounting::Entry *)entry.release());
                transaction_from_db->add_entry(std::move(to_add));
            }
            std::unique_ptr<HasTable> to_ret((HasTable *)transaction_from_db);
            return std::move(to_ret);
        }
    }
}