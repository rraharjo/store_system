#include "util/class/inventories_collection.hpp"

namespace util
{
    namespace baseclass
    {
        InventoryCollection::InventoryCollection() : Collection(util::enums::PrimaryKeyPrefix::INVENTORY,
                                                                util::InventoryTable::get_instance())
        {
            this->purchase_history_collection = std::make_unique<PurchaseEntriesCollection>();
            this->selling_history_collection = std::make_unique<SellingEntriesCollection>();
        }

        InventoryCollection::~InventoryCollection()
        {
#ifdef DEBUG
            std::cout << "Deleting Inventory Collection" << std::endl;
#endif
        }

        void InventoryCollection::insert_new_item(HasTable *new_item)
        {
            Collection::validate_insert(new_item);
            inventory::Inventory *new_inventory = (inventory::Inventory *)new_item;
            std::vector<std::string> parameter = {
                util::enums::primary_key_prefix_map[this->primary_key_prefix],
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
                // existing_inventory->get_db_code(),
                existing_inventory->get_item_code(),
                existing_inventory->get_name(),
                std::to_string(existing_inventory->get_selling_price()),
            };
            this->table->update_row(existing_inventory->get_db_code(), parameter);
            for (std::shared_ptr<inventory::PurchaseEntry> existing_entry : existing_inventory->get_purchase_entries())
            {
                this->purchase_history_collection->update_existing_item(existing_entry.get());
            }
            for (std::shared_ptr<inventory::SellingEntry> existing_entry : existing_inventory->get_selling_entries())
            {
                this->selling_history_collection->update_existing_item(existing_entry.get());
            }
        }

        std::unique_ptr<HasTable> InventoryCollection::get_from_database(std::string db_code)
        {
            std::string this_primary_key_prefix_string = util::enums::primary_key_prefix_map[this->primary_key_prefix];
            if (db_code.rfind(this_primary_key_prefix_string) != 0)
            {
                throw std::invalid_argument("Cannot get a " +
                                            db_code +
                                            " from " +
                                            this_primary_key_prefix_string +
                                            " table...\n");
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
            std::unique_ptr<inventory::Inventory> inventory_from_db =
                std::make_unique<inventory::Inventory>(record[0], record[1], record[2], std::stod(record[3]));
            conditions.clear();
            util::TableCondition equal_inventory_code;
            equal_inventory_code.col =
                util::enums::purchase_entry_table_columns[util::enums::PurchaseEntryTable::INVENTORYDBCODE];
            equal_inventory_code.comparator = util::TableComparator::EQUAL;
            equal_inventory_code.value = inventory_from_db->get_db_code();
            conditions.push_back(equal_inventory_code);
            std::vector<std::unique_ptr<util::baseclass::HasTable>> purchase_entries =
                this->purchase_history_collection.get()->get_from_database(conditions);
            for (std::unique_ptr<util::baseclass::HasTable> &purchase_entry : purchase_entries)
            {
                std::unique_ptr<inventory::PurchaseEntry> casted_ptr((inventory::PurchaseEntry *)purchase_entry.release());
                inventory_from_db->add_existing_purchase_entry(std::move(casted_ptr));
            }
            conditions.clear();
            equal_inventory_code.col = util::enums::selling_entry_table_columns[util::enums::SellingEntryTable::INVENTORYDBCODE];
            equal_inventory_code.comparator = util::TableComparator::EQUAL;
            equal_inventory_code.value = inventory_from_db->get_db_code();
            conditions.push_back(equal_inventory_code);
            std::vector<std::unique_ptr<util::baseclass::HasTable>> selling_entries =
                this->selling_history_collection.get()->get_from_database(conditions);
            for (std::unique_ptr<util::baseclass::HasTable> &selling_entry : selling_entries)
            {
                std::unique_ptr<inventory::SellingEntry> casted_ptr((inventory::SellingEntry *)selling_entry.release());
                inventory_from_db->add_existing_selling_entry(std::move(casted_ptr));
            }
            std::unique_ptr<HasTable> to_ret((HasTable *)inventory_from_db.release());
            return std::move(to_ret);
        }

        std::vector<std::unique_ptr<HasTable>> InventoryCollection::get_from_database(
            std::vector<util::TableCondition> &conditions)
        {
            std::vector<std::unique_ptr<HasTable>> to_ret;
            std::vector<std::vector<std::string>> records = this->table->get_records(conditions);
            for (std::vector<std::string> &record : records)
            {
                std::unique_ptr<inventory::Inventory> to_add =
                    std::make_unique<inventory::Inventory>(record[0],
                                                           record[1],
                                                           record[2],
                                                           std::stod(record[3]));
                std::vector<util::TableCondition> inventory_condition;
                util::TableCondition equal_inventory_code;
                equal_inventory_code.col =
                    util::enums::purchase_entry_table_columns[util::enums::PurchaseEntryTable::INVENTORYDBCODE];
                equal_inventory_code.comparator = util::TableComparator::EQUAL;
                equal_inventory_code.value = to_add->get_db_code();
                inventory_condition.push_back(equal_inventory_code);
                std::vector<std::unique_ptr<util::baseclass::HasTable>> purchase_entries = this->purchase_history_collection.get()->get_from_database(inventory_condition);
                for (std::unique_ptr<util::baseclass::HasTable> &purchase_entry : purchase_entries)
                {
                    std::unique_ptr<inventory::PurchaseEntry> casted_ptr((inventory::PurchaseEntry *)purchase_entry.release());
                    to_add->add_existing_purchase_entry(std::move(casted_ptr));
                }
                inventory_condition.clear();
                equal_inventory_code.col =
                    util::enums::selling_entry_table_columns[util::enums::SellingEntryTable::INVENTORYDBCODE];
                equal_inventory_code.comparator = util::TableComparator::EQUAL;
                equal_inventory_code.value = to_add->get_db_code();
                inventory_condition.push_back(equal_inventory_code);
                std::vector<std::unique_ptr<util::baseclass::HasTable>> selling_entries =
                    this->selling_history_collection.get()->get_from_database(inventory_condition);
                for (std::unique_ptr<util::baseclass::HasTable> &selling_entry : selling_entries)
                {
                    std::unique_ptr<inventory::SellingEntry> casted_ptr((inventory::SellingEntry *)selling_entry.release());
                    to_add->add_existing_selling_entry(std::move(casted_ptr));
                }
                std::unique_ptr<HasTable> casted_ptr((HasTable *)to_add.release());
                to_ret.push_back(std::move(casted_ptr));
            }
            return to_ret;
        }
    };
}