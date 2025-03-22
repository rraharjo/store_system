#include "util/class/equipment_collection.hpp"

namespace util
{
    namespace baseclass
    {
        EquipmentCollection::EquipmentCollection()
            : Collection(util::enums::PrimaryKeyPrefix::EQUIPMENT,
                         util::AssetsTable::get_instance())
        {
            this->purchase_history_collection = std::make_unique<PurchaseEntriesCollection>();
            this->selling_history_collection = std::make_unique<SellingEntriesCollection>();
        }

        EquipmentCollection::~EquipmentCollection()
        {
#ifdef DEBUG
            std::cout << "Deleting Equipment Collection" << std::endl;
#endif
        }

        void EquipmentCollection::insert_new_item(HasTable *new_item)
        {
            Collection::validate_insert(new_item);
            inventory::Equipment *new_equipment = (inventory::Equipment *)new_item;
            std::vector<std::string> parameter = {
                util::enums::primary_key_prefix_map[this->primary_key_prefix],
                new_equipment->get_name(),
                std::to_string(new_equipment->get_total_value()),
                std::to_string(new_equipment->get_residual_value()),
                std::to_string(new_equipment->get_year_useful_life()),
                new_equipment->get_date_bought()->to_db_format(),
                new_equipment->get_last_depreciation_date() ? new_equipment->get_last_depreciation_date()->to_db_format() : "NULL",
                new_equipment->get_expiry_date() ? new_equipment->get_expiry_date()->to_db_format() : "NULL",
            };
            std::vector<std::string> result = this->table->insert_row(parameter);
            Collection::set_db_code(new_equipment, result[0]);
            for (std::shared_ptr<inventory::PurchaseEntry> new_entry : new_equipment->get_purchase_entries())
            {
                this->purchase_history_collection->set_item_db_code(new_entry.get(), new_equipment->get_db_code());
                this->purchase_history_collection->update_existing_item(new_entry.get());
            }
            for (std::shared_ptr<inventory::SellingEntry> new_entry : new_equipment->get_selling_entries())
            {
                this->selling_history_collection->set_item_db_code(new_entry.get(), new_equipment->get_db_code());
                this->purchase_history_collection->update_existing_item(new_entry.get());
            }
        }

        void EquipmentCollection::update_existing_item(HasTable *existing_item)
        {
            Collection::validate_update(existing_item);
            inventory::Equipment *existing_equipment = (inventory::Equipment *)existing_item;
            std::vector<std::string> parameter = {
                existing_equipment->get_name(),
                std::to_string(existing_equipment->get_total_value()),
                std::to_string(existing_equipment->get_residual_value()),
                std::to_string(existing_equipment->get_year_useful_life()),
                existing_equipment->get_date_bought()->to_db_format(),
                existing_equipment->get_last_depreciation_date() ? existing_equipment->get_last_depreciation_date()->to_db_format() : "NULL",
                existing_equipment->get_expiry_date() ? existing_equipment->get_expiry_date()->to_db_format() : "NULL",
            };
            this->table->update_row(existing_equipment->get_db_code(), parameter);
            for (std::shared_ptr<inventory::PurchaseEntry> entry : existing_equipment->get_purchase_entries())
            {
                if (entry->get_db_code() == "")
                {
                    this->purchase_history_collection->insert_new_item(entry.get());
                }
                else
                {
                    this->purchase_history_collection->update_existing_item(entry.get());
                }
            }
            for (std::shared_ptr<inventory::SellingEntry> entry : existing_equipment->get_selling_entries())
            {
                if (entry->get_db_code() == "")
                {
                    this->selling_history_collection->insert_new_item(entry.get());
                }
                else
                {
                    this->selling_history_collection->update_existing_item(entry.get());
                }
            }
        }

        std::unique_ptr<HasTable> EquipmentCollection::get_from_database(std::string db_code)
        {
            std::string this_primary_key_prefix_string = util::enums::primary_key_prefix_map[this->primary_key_prefix];
            if (db_code.rfind(this_primary_key_prefix_string) != 0)
            {
                throw std::invalid_argument("Cannot get a " + db_code + " from " + this_primary_key_prefix_string + " table...\n");
            }
            std::vector<util::TableCondition> conditions;
            util::TableCondition equal_db_code;
            equal_db_code.col = util::enums::assets_table_columns[util::enums::AssetsTable::DATABASECODE];
            equal_db_code.comparator = TableComparator::EQUAL;
            equal_db_code.value = db_code;
            conditions.push_back(equal_db_code);
            std::vector<std::vector<std::string>> records = this->table->get_records(conditions);
            if (records.empty())
            {
                throw std::invalid_argument("No item with code " + db_code + " in the database");
            }
            std::vector<std::string> record = records[0];
            std::unique_ptr<util::Date> purchase = NULL, depreciation_date = NULL, sold = NULL;
            if (record[5] != "")
            {
                purchase = std::make_unique<util::Date>(record[5], "%Y-%m-%d");
            }
            if (record[6] != "")
            {
                depreciation_date = std::make_unique<util::Date>(record[6], "%Y-%m-%d");
            }
            if (record[7] != "")
            {
                sold = std::make_unique<util::Date>(record[6], "%Y-%m-%d");
            }
            std::unique_ptr<inventory::Equipment> new_equipment =
                std::make_unique<inventory::Equipment>(record[0],
                                                       record[1],
                                                       "",
                                                       std::stod(record[2]),
                                                       std::stod(record[3]),
                                                       std::stoi(record[4]),
                                                       std::move(purchase),
                                                       std::move(depreciation_date),
                                                       std::move(sold));
            conditions.clear();
            util::TableCondition equal_asset_code;
            equal_asset_code.col = util::enums::purchase_entry_table_columns[util::enums::PurchaseEntryTable::ASSETSCODE];
            equal_asset_code.comparator = util::TableComparator::EQUAL;
            equal_asset_code.value = new_equipment->get_db_code();
            conditions.push_back(equal_asset_code);
            std::vector<std::unique_ptr<util::baseclass::HasTable>> purchase_entries = this->purchase_history_collection.get()->get_from_database(conditions);
            for (std::unique_ptr<util::baseclass::HasTable> &purchase_entry : purchase_entries)
            {
                std::unique_ptr<inventory::PurchaseEntry> casted_ptr((inventory::PurchaseEntry *)purchase_entry.release());
                new_equipment->add_existing_purchase_entry(std::move(casted_ptr));
            }

            conditions.clear();
            equal_asset_code.col = util::enums::selling_entry_table_columns[util::enums::SellingEntryTable::ASSETSCODE];
            equal_asset_code.comparator = util::TableComparator::EQUAL;
            equal_asset_code.value = new_equipment->get_db_code();
            conditions.push_back(equal_asset_code);
            std::vector<std::unique_ptr<util::baseclass::HasTable>> selling_entries = this->selling_history_collection.get()->get_from_database(conditions);
            for (std::unique_ptr<util::baseclass::HasTable> &selling_entry : selling_entries)
            {
                std::unique_ptr<inventory::SellingEntry> casted_ptr((inventory::SellingEntry *)selling_entry.release());
                new_equipment->add_existing_selling_entry(std::move(casted_ptr));
            }
            std::unique_ptr<HasTable> to_ret((HasTable *)new_equipment.release());
            return std::move(to_ret);
        }
        std::vector<std::unique_ptr<HasTable>> EquipmentCollection::get_from_database(std::vector<util::TableCondition> &conditions)
        {
            std::vector<std::unique_ptr<HasTable>> to_ret;
            std::vector<std::vector<std::string>> records = this->table->get_records(conditions);
            for (std::vector<std::string> &record : records)
            {
                std::unique_ptr<util::Date> purchase = NULL, depreciation_date = NULL, sold = NULL;
                if (record[5] != "")
                {
                    purchase = std::make_unique<util::Date>(record[5], "%Y-%m-%d");
                }
                if (record[6] != "")
                {
                    depreciation_date = std::make_unique<util::Date>(record[6], "%Y-%m-%d");
                }
                if (record[7] != "")
                {
                    sold = std::make_unique<util::Date>(record[6], "%Y-%m-%d");
                }
                std::unique_ptr<inventory::Equipment> equipment_from_db =
                    std::make_unique<inventory::Equipment>(record[0],
                                                           record[1],
                                                           "",
                                                           std::stod(record[2]),
                                                           std::stod(record[3]),
                                                           std::stoi(record[4]),
                                                           std::move(purchase),
                                                           std::move(depreciation_date),
                                                           std::move(sold));
                std::unique_ptr<HasTable> to_add((HasTable *)equipment_from_db.release());
                to_ret.push_back(std::move(to_add));
            }
            return to_ret;
        }
    };
}