#include "util/class/selling_transaction_collection.hpp"

namespace util
{
    namespace baseclass
    {
        SellingTransactionCollection::SellingTransactionCollection() : Collection(util::enums::primary_key_codes_map[util::enums::PrimaryKeyCodes::SELLINGTRANSACTION],
                                                                                    util::SellingTransactionTable::get_instance())
        {
            this->selling_entries = std::unique_ptr<SellingEntriesCollection>(new SellingEntriesCollection());
        }

        void SellingTransactionCollection::insert_new_item(HasTable *new_item)
        {
            Collection::validate_insert(new_item);
            store::SellingTransaction *new_transaction = (store::SellingTransaction *)new_item;
            std::vector<std::string> parameter = {
                this->primary_key,
                new_transaction->get_date()->to_db_format(),
                std::to_string(new_transaction->get_paid_cash()),
                std::to_string(new_transaction->get_paid_credit()),
                new_transaction->is_finished ? "true" : "false",
            };
            std::vector<std::string> result = this->table->insert_row(parameter);
            Collection::set_db_code(new_transaction, result[0]);
            for (inventory::Entry *entry : new_transaction->get_all_entries())
            {
                inventory::SellingEntry *new_entry = (inventory::SellingEntry *)entry;
                new_entry->set_transaction_db_code(new_transaction->get_db_code());
                this->selling_entries->insert_new_item(new_entry);
            }
        }

        void SellingTransactionCollection::update_existing_item(HasTable *existing_item)
        {
            Collection::validate_update(existing_item);
            store::SellingTransaction *existing_transaction = (store::SellingTransaction *)existing_item;
            std::vector<std::string> parameter = {
                this->primary_key,
                existing_transaction->get_date()->to_db_format(),
                std::to_string(existing_transaction->get_paid_cash()),
                std::to_string(existing_transaction->get_paid_credit()),
                existing_transaction->is_finished ? "true" : "false",
            };
            this->table->update_row(existing_transaction->get_db_code(), parameter);
            for (inventory::Entry *existing_entry : existing_transaction->get_all_entries())
            {
                this->selling_entries->update_existing_item((inventory::SellingEntry *)existing_entry);
            }
        }

        HasTable *SellingTransactionCollection::get_from_database(std::string db_code)
        {
            if (db_code.rfind(this->primary_key) != 0)
            {
                throw std::invalid_argument("Cannot get a " + db_code + " from " + this->primary_key + " table...\n");
            }
            std::vector<util::TableCondition> conditions;
            util::TableCondition equal_db_code;
            equal_db_code.col = util::enums::selling_transaction_table_columns[util::enums::SellingTransactionTable::DATABASECODE];
            equal_db_code.comparator = TableComparator::EQUAL;
            equal_db_code.value = db_code;
            conditions.push_back(equal_db_code);
            std::vector<std::vector<std::string>> records = this->table->get_records(conditions);
            if (records.empty())
            {
                throw std::invalid_argument("No item with code " + db_code + " in the database");
            }
            std::vector<std::string> record = records[0];
            util::Date *transaction_date = new util::Date(record[1], "%Y-%m-%d");
            store::SellingTransaction *transaction_from_db = new store::SellingTransaction(record[0],
                                                                                             transaction_date,
                                                                                             std::stod(record[2]),
                                                                                             std::stod(record[3]));
            conditions.clear();
            util::TableCondition equal_transaction_code;
            equal_transaction_code.col = util::enums::selling_entry_table_columns[util::enums::SellingEntryTable::ASSETSCODE];
            equal_transaction_code.comparator = util::TableComparator::EQUAL;
            equal_transaction_code.value = transaction_from_db->get_db_code();
            conditions.push_back(equal_transaction_code);
            std::vector<util::baseclass::HasTable *> entries = this->selling_entries.get()->get_from_database(conditions);
            for (util::baseclass::HasTable *entry : entries)
            {
                transaction_from_db->add_entry((inventory::SellingEntry *)entry);
            }
            return transaction_from_db;
        }

    };
}