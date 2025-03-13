#include "util/class/inventories_collection.hpp"

namespace util
{
    namespace baseclass
    {
        InventoryCollection::InventoryCollection() : Collection(util::enums::primary_key_codes_map[util::enums::PrimaryKeyCodes::INVENTORY],
                                                                util::InventoryTable::get_instance())
        {
            this->purchase_history_collection = std::unique_ptr<PurchaseEntriesCollection>(new PurchaseEntriesCollection());
            this->selling_history_collection = std::unique_ptr<SellingEntriesCollection>(new SellingEntriesCollection());
        }

        void InventoryCollection::insert_new_item(HasTable *new_item)
        {
            Collection::validate_insert(new_item);
            inventory::Inventory *new_inventory = (inventory::Inventory *)new_item;
            std::vector<std::string> parameter = {
                this->primary_key,
                new_inventory->get_item_code(),
                new_inventory->get_name(),
                std::to_string(new_inventory->get_selling_price()),
            };
            std::vector<std::string> result = this->table->insert_row(parameter);
            Collection::set_db_code(new_inventory, result[0]);
        }

        void InventoryCollection::update_existing_item(HasTable *existing_item)
        {
            Collection::validate_update(existing_item);
            inventory::Inventory *existing_inventory = (inventory::Inventory *)existing_item;
            std::vector<std::string> parameter = {
                existing_inventory->get_db_code(),
                existing_inventory->get_item_code(),
                existing_inventory->get_name(),
                std::to_string(existing_inventory->get_selling_price()),
            };
            this->table->update_row(existing_inventory->get_db_code(), parameter);
            for (inventory::PurchaseEntry *existing_entry : existing_inventory->get_purchase_entries())
            {
                this->purchase_history_collection->update_existing_item(existing_entry);
            }
            for (inventory::SellingEntry *existing_entry : existing_inventory->get_selling_entries())
            {
                this->selling_history_collection->update_existing_item(existing_entry);
            }
        }

        HasTable *InventoryCollection::get_from_database(std::string db_code)
        {
            if (db_code.rfind(this->primary_key) != 0)
            {
                throw std::invalid_argument("Cannot get a " + db_code + " from " + this->primary_key + " table...\n");
            }
            std::vector<util::TableCondition> conditions;
            util::TableCondition equal_db_code;
            equal_db_code.col = util::enums::inventory_table_columns[util::enums::InventoryTable::DATABASECODE];
            equal_db_code.comparator = TableComparator::EQUAL;
            equal_db_code.value = db_code;
            conditions.push_back(equal_db_code);
            std::vector<std::vector<std::string>> records = this->table->get_records(conditions);
            if (records.empty())
            {
                throw std::invalid_argument("No item with code " + db_code + " in the database");
            }
            std::vector<std::string> record = records[0];
            inventory::Inventory *inventory_from_db = new inventory::Inventory(record[0], record[1], record[2], std::stod(record[3]));
            conditions.clear();
            util::TableCondition equal_inventory_code;
            equal_inventory_code.col = util::enums::purchase_entry_table_columns[util::enums::PurchaseEntryTable::ASSETSCODE];
            equal_inventory_code.comparator = util::TableComparator::EQUAL;
            equal_inventory_code.value = inventory_from_db->get_db_code();
            conditions.push_back(equal_inventory_code);
            std::vector<util::baseclass::HasTable *> purchase_entries = this->purchase_history_collection.get()->get_from_database(conditions);
            for (util::baseclass::HasTable *purchase_entry : purchase_entries)
            {
                inventory_from_db->add_existing_purchase_entry((inventory::PurchaseEntry *)purchase_entry);
            }
            conditions.clear();
            equal_inventory_code.col = util::enums::selling_entry_table_columns[util::enums::SellingEntryTable::ASSETSCODE];
            equal_inventory_code.comparator = util::TableComparator::EQUAL;
            equal_inventory_code.value = inventory_from_db->get_db_code();
            conditions.push_back(equal_inventory_code);
            std::vector<util::baseclass::HasTable *> selling_entries = this->selling_history_collection.get()->get_from_database(conditions);
            for (util::baseclass::HasTable *selling_entry : selling_entries)
            {
                inventory_from_db->add_existing_selling_entry((inventory::SellingEntry *)selling_entry);
            }
            return inventory_from_db;
        }

        std::vector<HasTable *> InventoryCollection::get_from_database(std::vector<util::TableCondition> &conditions)
        {
            std::vector<HasTable *> to_ret;
            std::vector<std::vector<std::string>> records = this->table->get_records(conditions);
            for (std::vector<std::string> &record : records){
                to_ret.push_back(new inventory::Inventory(record[0], record[1], record[2], std::stod(record[3])));
            }
            return to_ret;
        }
    };
}