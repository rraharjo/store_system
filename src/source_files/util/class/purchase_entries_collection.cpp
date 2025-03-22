#include "util/class/purchase_entries_collection.hpp"

namespace util
{
    namespace baseclass
    {
        PurchaseEntriesCollection::PurchaseEntriesCollection()
            : Collection(util::enums::PrimaryKeyPrefix::PURCHASEENTRY,
                         util::PurchaseEntryTable::get_instance())
        {
        }

        PurchaseEntriesCollection::~PurchaseEntriesCollection()
        {
#ifdef DEBUG
            std::cout << "Deleting Purchase Entries Collection" << std::endl;
#endif
        }

        void PurchaseEntriesCollection::insert_new_item(HasTable *new_item)
        {
            Collection::validate_insert(new_item);
            inventory::PurchaseEntry *new_entry = (inventory::PurchaseEntry *)new_item;
            std::vector<std::string> parameter = {
                util::enums::primary_key_prefix_map[this->primary_key_prefix],
                new_entry->get_sellable_db_code() == "" ? "NULL" : new_entry->get_sellable_db_code(),
                new_entry->get_properties_db_code() == "" ? "NULL" : new_entry->get_properties_db_code(),
                new_entry->get_transaction_db_code(),
                std::to_string(new_entry->get_price()),
                std::to_string(new_entry->get_qty()),
                std::to_string(new_entry->get_available_qty()),
            };
            std::vector<std::string> res = this->table->insert_row(parameter);
            Collection::set_db_code(new_item, res[0]);
        }

        void PurchaseEntriesCollection::update_existing_item(HasTable *existing_item)
        {
            Collection::validate_update(existing_item);
            inventory::PurchaseEntry *existing_entry = (inventory::PurchaseEntry *)existing_item;
            std::string inv_db_code, asset_db_code;
            inv_db_code = existing_entry->get_sellable_db_code();
            asset_db_code = existing_entry->get_properties_db_code();
            if (inv_db_code == "")
            {
                inv_db_code = "NULL";
            }
            if (asset_db_code == "")
            {
                asset_db_code = "NULL";
            }
            std::vector<std::string> values = {
                inv_db_code,
                asset_db_code,
                existing_entry->get_transaction_db_code(),
                std::to_string(existing_entry->get_price()),
                std::to_string(existing_entry->get_qty()),
                std::to_string(existing_entry->get_available_qty())};
            this->table->update_row(existing_entry->get_db_code(), values);
        }

        std::unique_ptr<HasTable> PurchaseEntriesCollection::get_from_database(std::string db_code)
        {
            std::string this_primary_key_prefix_string = util::enums::primary_key_prefix_map[this->primary_key_prefix];
            if (db_code.rfind(this_primary_key_prefix_string) != 0)
            {
                throw std::invalid_argument("Cannot get a " + db_code + " from " + this_primary_key_prefix_string + " table...\n");
            }
            std::vector<util::TableCondition> conditions;
            util::TableCondition equal_db_code;
            equal_db_code.col = util::enums::purchase_entry_table_columns[util::enums::PurchaseEntryTable::DATABASECODE];
            equal_db_code.comparator = TableComparator::EQUAL;
            equal_db_code.value = db_code;
            conditions.push_back(equal_db_code);
            std::vector<std::vector<std::string>> records = this->table->get_records(conditions);
            if (records.empty())
            {
                throw std::invalid_argument("No item with code " + db_code + " in the database");
            }
            std::vector<std::string> record = records[0];
            int item_index;
            if (record[1] == "")
            {
                item_index = 2;
            }
            else
            {
                item_index = 1;
            }
            std::unique_ptr<inventory::PurchaseEntry> purchase_entry_from_db =
                std::make_unique<inventory::PurchaseEntry>(record[0],
                                                           record[item_index],
                                                           record[3],
                                                           std::stod(record[4]),
                                                           std::stoi(record[5]),
                                                           std::stoi(record[6]));
            std::unique_ptr<HasTable> to_ret((HasTable *)purchase_entry_from_db.release());
            return std::move(to_ret);
        }

        std::vector<std::unique_ptr<HasTable>> PurchaseEntriesCollection::get_from_database(std::vector<util::TableCondition> &conditions)
        {
            std::vector<std::unique_ptr<HasTable>> to_ret;
            std::vector<std::vector<std::string>> records = this->table->get_records(conditions);
            for (std::vector<std::string> &record : records)
            {
                int item_index;
                if (record[1] == "")
                {
                    item_index = 2;
                }
                else
                {
                    item_index = 1;
                }
                std::unique_ptr<inventory::PurchaseEntry> purchase_entry_from_db =
                    std::make_unique<inventory::PurchaseEntry>(record[0],
                                                               record[item_index],
                                                               record[3],
                                                               std::stod(record[4]),
                                                               std::stoi(record[5]),
                                                               std::stoi(record[6]));
                std::unique_ptr<HasTable> to_add((HasTable *)purchase_entry_from_db.release());
                to_ret.push_back(std::move(to_add));
            }
            return to_ret;
        }

        void PurchaseEntriesCollection::set_item_db_code(inventory::PurchaseEntry *new_entry, std::string db_code)
        {
            new_entry->set_item_db_code(db_code);
        }
    };
}