#include "util/class/selling_entries_collection.hpp"

namespace util
{
    namespace baseclass
    {
        SellingEntriesCollection::SellingEntriesCollection()
            : Collection(util::enums::PrimaryKeyPrefix::SELLINGENTRY,
                         util::SellingEntryTable::get_instance())
        {
        }

        void SellingEntriesCollection::insert_new_item(HasTable *new_item)
        {
            Collection::validate_insert(new_item);
            inventory::SellingEntry *new_entry = (inventory::SellingEntry *)new_item;
            std::vector<std::string> parameter = {
                util::enums::primary_key_prefix_map[this->primary_key_prefix],
                new_entry->get_sellable_db_code() == "" ? "NULL" : new_entry->get_sellable_db_code(),
                new_entry->get_properties_db_code() == "" ? "NULL" : new_entry->get_properties_db_code(),
                new_entry->get_transaction_db_code(),
                std::to_string(new_entry->get_price()),
                std::to_string(new_entry->get_qty()),
            };
            this->table->insert_row(parameter);
        }

        HasTable *SellingEntriesCollection::get_from_database(std::string db_code)
        {
            std::string this_primary_key_prefix_string = util::enums::primary_key_prefix_map[this->primary_key_prefix];
            if (db_code.rfind(this_primary_key_prefix_string) != 0)
            {
                throw std::invalid_argument("Cannot get a " + db_code + " from " + this_primary_key_prefix_string + " table...\n");
            }
            std::vector<util::TableCondition> conditions;
            util::TableCondition equal_db_code;
            equal_db_code.col = util::enums::selling_entry_table_columns[util::enums::SellingEntryTable::DATABASECODE];
            equal_db_code.comparator = TableComparator::EQUAL;
            equal_db_code.value = db_code;
            conditions.push_back(equal_db_code);
            std::vector<std::vector<std::string>> records = this->table->get_records(conditions);
            if (records.empty())
            {
                throw std::invalid_argument("No item with code " + db_code + " in the database\n");
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
            inventory::PurchaseEntry *purchase_entry_from_db = new inventory::PurchaseEntry(record[0],
                                                                                            record[item_index],
                                                                                            record[3],
                                                                                            std::stod(record[4]),
                                                                                            std::stoi(record[5]),
                                                                                            std::stoi(record[6]));
            return purchase_entry_from_db;
        }

        void SellingEntriesCollection::update_existing_item(HasTable *existing_item)
        {
            inventory::SellingEntry *existing_entry = (inventory::SellingEntry *)existing_item;
            std::string inv_db_code, asset_db_code;
            if (existing_entry->get_properties_db_code().rfind(util::enums::primary_key_prefix_map[util::enums::PrimaryKeyPrefix::INVENTORY]) == 0)
            {
                inv_db_code = existing_entry->get_properties_db_code();
                asset_db_code = "";
            }
            else
            {
                inv_db_code = "";
                asset_db_code = existing_entry->get_properties_db_code();
            }
            std::vector<std::string> values = {
                inv_db_code,
                asset_db_code,
                existing_entry->get_transaction_db_code(),
                std::to_string(existing_entry->get_price()),
                std::to_string(existing_entry->get_qty())};
            this->table->update_row(existing_entry->get_db_code(), values);
        };

        std::vector<HasTable *> SellingEntriesCollection::get_from_database(std::vector<util::TableCondition> &conditions)
        {
            std::vector<HasTable *> to_ret;
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
                inventory::SellingEntry *selling_entry_from_db = new inventory::SellingEntry(record[0],
                                                                                             record[item_index],
                                                                                             record[3],
                                                                                             std::stod(record[4]),
                                                                                             std::stoi(record[5]));
                to_ret.push_back(selling_entry_from_db);
            }
            return to_ret;
        }

        void SellingEntriesCollection::set_item_db_code(inventory::SellingEntry *new_entry, std::string db_code)
        {
            new_entry->set_item_db_code(db_code);
        }
    };
}