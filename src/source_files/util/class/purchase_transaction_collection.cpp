#include "util/class/purchase_transaction_collection.hpp"

namespace util
{
    namespace baseclass
    {
        PurchaseTransactionCollection::PurchaseTransactionCollection()
            : Collection(util::enums::PrimaryKeyPrefix::PURCHASETRANSACTION,
                         util::PurchaseTransactionTable::get_instance())
        {
            this->purchase_entries = std::make_unique<PurchaseEntriesCollection>();
        }

        PurchaseTransactionCollection::~PurchaseTransactionCollection()
        {
#ifdef DEBUG
            std::cout << "Deleting Purchase Transaction Collection" << std::endl;
#endif
        }

        void PurchaseTransactionCollection::insert_new_item(HasTable *new_item)
        {
            Collection::validate_insert(new_item);
            store::PurchaseTransaction *new_transaction = (store::PurchaseTransaction *)new_item;
            std::vector<std::string> parameter = {
                util::enums::primary_key_prefix_map[this->primary_key_prefix],
                new_transaction->get_date()->to_db_format(),
                new_transaction->get_seller(),
                std::to_string(new_transaction->get_paid_cash()),
                std::to_string(new_transaction->get_paid_credit()),
                new_transaction->is_finished ? "true" : "false",
            };
            std::vector<std::string> result = this->table->insert_row(parameter);
            Collection::set_db_code(new_transaction, result[0]);
            for (std::shared_ptr<inventory::Entry> entry : new_transaction->get_all_entries())
            {
                inventory::PurchaseEntry *new_entry = (inventory::PurchaseEntry *)entry.get();
                new_entry->set_transaction_db_code(new_transaction->get_db_code());
                this->purchase_entries->insert_new_item(new_entry);
            }
        }

        void PurchaseTransactionCollection::update_existing_item(HasTable *existing_item)
        {
            Collection::validate_update(existing_item);
            store::PurchaseTransaction *existing_transaction = (store::PurchaseTransaction *)existing_item;
            std::vector<std::string> parameter = {
                util::enums::primary_key_prefix_map[this->primary_key_prefix],
                existing_transaction->get_date()->to_db_format(),
                existing_transaction->get_seller(),
                std::to_string(existing_transaction->get_paid_cash()),
                std::to_string(existing_transaction->get_paid_credit()),
                existing_transaction->is_finished ? "true" : "false",
            };
            this->table->update_row(existing_transaction->get_db_code(), parameter);
            for (std::shared_ptr<inventory::Entry> existing_entry : existing_transaction->get_all_entries())
            {
                this->purchase_entries->update_existing_item((inventory::PurchaseEntry *)existing_entry.get());
            }
        }

        std::unique_ptr<HasTable> PurchaseTransactionCollection::get_from_database(std::string db_code)
        {
            std::string this_primary_key_prefix_string = util::enums::primary_key_prefix_map[this->primary_key_prefix];
            if (db_code.rfind(this_primary_key_prefix_string) != 0)
            {
                throw std::invalid_argument("Cannot get a " + db_code + " from " + this_primary_key_prefix_string + " table...\n");
            }
            std::vector<util::TableCondition> conditions;
            util::TableCondition equal_db_code;
            equal_db_code.col = util::enums::purchase_transaction_table_columns[util::enums::PurchaseTransactionTable::DATABASECODE];
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
            std::unique_ptr<store::PurchaseTransaction> transaction_from_db =
                std::make_unique<store::PurchaseTransaction>(record[0],
                                                             record[2],
                                                             std::move(transaction_date),
                                                             std::stod(record[3]),
                                                             std::stod(record[4]));
            conditions.clear();
            util::TableCondition equal_transaction_code;
            equal_transaction_code.col = util::enums::purchase_entry_table_columns[util::enums::PurchaseEntryTable::ASSETSCODE];
            equal_transaction_code.comparator = util::TableComparator::EQUAL;
            equal_transaction_code.value = transaction_from_db->get_db_code();
            conditions.push_back(equal_transaction_code);
            std::vector<std::unique_ptr<util::baseclass::HasTable>> entries = this->purchase_entries.get()->get_from_database(conditions);
            for (std::unique_ptr<util::baseclass::HasTable> &entry : entries)
            {
                std::unique_ptr<inventory::Entry> to_add((inventory::Entry *)entry.release());
                transaction_from_db->add_entry(std::move(to_add));
            }
            std::unique_ptr<HasTable> to_ret((HasTable *)transaction_from_db.release());
            return std::move(to_ret);
        }

        std::vector<std::unique_ptr<HasTable>> PurchaseTransactionCollection::get_from_database(std::vector<util::TableCondition> &conditions)
        {
            std::vector<std::unique_ptr<HasTable>> to_ret;
            std::vector<std::vector<std::string>> records = this->table->get_records(conditions);
            conditions.clear();
            for (std::vector<std::string> &record : records)
            {
                std::unique_ptr<util::Date> transaction_date = std::make_unique<util::Date>(record[1], "%Y-%m-%d");
                std::unique_ptr<store::PurchaseTransaction> transaction_from_db =
                    std::make_unique<store::PurchaseTransaction>(record[0],
                                                                 record[2],
                                                                 std::move(transaction_date),
                                                                 std::stod(record[3]),
                                                                 std::stod(record[4]));
                util::TableCondition equal_transaction_code;
                equal_transaction_code.col = util::enums::purchase_entry_table_columns[util::enums::PurchaseEntryTable::ASSETSCODE];
                equal_transaction_code.comparator = util::TableComparator::EQUAL;
                equal_transaction_code.value = transaction_from_db->get_db_code();
                conditions.push_back(equal_transaction_code);
                std::vector<std::unique_ptr<util::baseclass::HasTable>> entries = this->purchase_entries.get()->get_from_database(conditions);
                for (std::unique_ptr<util::baseclass::HasTable> &entry : entries)
                {
                    std::unique_ptr<inventory::Entry> to_add((inventory::Entry *)entry.release());
                    transaction_from_db->add_entry(std::move(to_add));
                }
                std::unique_ptr<HasTable> to_add((HasTable *)transaction_from_db.release());
                to_ret.push_back(std::move(to_add));
            }
            return to_ret;
        }
    };
}