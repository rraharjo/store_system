#include "util/class/selling_transaction_collection.hpp"

namespace util
{
    namespace baseclass
    {
        SellingTransactionCollection::SellingTransactionCollection()
            : Collection(util::enums::PrimaryKeyPrefix::SELLINGTRANSACTION,
                         util::SellingTransactionTable::get_instance())
        {
            this->selling_entries = std::make_unique<SellingEntriesCollection>();
        }

        SellingTransactionCollection::~SellingTransactionCollection()
        {
#ifdef DEBUG
            std::cout << "Deleting Selling Transaction Collection" << std::endl;
#endif
        }

        void SellingTransactionCollection::insert_new_item(HasTable *new_item)
        {
            Collection::validate_insert(new_item);
            store::SellingTransaction *new_transaction = (store::SellingTransaction *)new_item;
            std::vector<std::string> parameter = {
                util::enums::primary_key_prefix_map[this->primary_key_prefix],
                new_transaction->get_date()->to_db_format(),
                std::to_string(new_transaction->get_paid_cash()),
                std::to_string(new_transaction->get_paid_credit()),
                new_transaction->is_finished ? "true" : "false",
            };
            std::vector<std::string> result = this->table->insert_row(parameter);
            Collection::set_db_code(new_transaction, result[0]);
            for (std::shared_ptr<inventory::Entry> entry : new_transaction->get_all_entries())
            {
                inventory::SellingEntry *new_entry = (inventory::SellingEntry *)entry.get();
                new_entry->set_transaction_db_code(new_transaction->get_db_code());
                this->selling_entries->insert_new_item(new_entry);
            }
        }

        void SellingTransactionCollection::update_existing_item(HasTable *existing_item)
        {
            Collection::validate_update(existing_item);
            store::SellingTransaction *existing_transaction = (store::SellingTransaction *)existing_item;
            std::vector<std::string> parameter = {
                existing_transaction->get_date()->to_db_format(),
                std::to_string(existing_transaction->get_paid_cash()),
                std::to_string(existing_transaction->get_paid_credit()),
                existing_transaction->is_finished ? "true" : "false",
            };
            this->table->update_row(existing_transaction->get_db_code(), parameter);
            for (std::shared_ptr<inventory::Entry> existing_entry : existing_transaction->get_all_entries())
            {
                this->selling_entries->update_existing_item((inventory::SellingEntry *)existing_entry.get());
            }
        }

        std::unique_ptr<HasTable> SellingTransactionCollection::get_from_database(std::string db_code)
        {
            std::string this_primary_key_prefix_string = util::enums::primary_key_prefix_map[this->primary_key_prefix];
            if (db_code.rfind(this_primary_key_prefix_string) != 0)
            {
                throw std::invalid_argument("Cannot get a " + db_code + " from " + this_primary_key_prefix_string + " table...\n");
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
            std::unique_ptr<util::Date> transaction_date = std::make_unique<util::Date>(record[1], "%Y-%m-%d");
            std::unique_ptr<store::SellingTransaction> transaction_from_db =
                std::make_unique<store::SellingTransaction>(record[0],
                                                            std::move(transaction_date),
                                                            std::stod(record[2]),
                                                            std::stod(record[3]));
            conditions.clear();
            util::TableCondition equal_transaction_code;
            equal_transaction_code.col = util::enums::selling_entry_table_columns[util::enums::SellingEntryTable::ASSETSCODE];
            equal_transaction_code.comparator = util::TableComparator::EQUAL;
            equal_transaction_code.value = transaction_from_db->get_db_code();
            conditions.push_back(equal_transaction_code);
            std::vector<std::unique_ptr<util::baseclass::HasTable>> entries = this->selling_entries.get()->get_from_database(conditions);
            for (std::unique_ptr<util::baseclass::HasTable> &entry : entries)
            {
                std::unique_ptr<inventory::SellingEntry> to_add((inventory::SellingEntry *)entry.release());
                transaction_from_db->add_entry(std::move(to_add));
            }
            std::unique_ptr<HasTable> to_ret((HasTable *)transaction_from_db.release());
            return std::move(to_ret);
        }

    };
}