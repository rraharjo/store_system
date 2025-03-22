#include "inventory/inventory_system.hpp"
using namespace inventory;

std::unique_ptr<InventorySystem> InventorySystem::instance = NULL;

InventorySystem *InventorySystem::get_instance()
{
    if (InventorySystem::instance.get() == NULL)
    {
        InventorySystem::instance.reset(new InventorySystem());
        InventorySystem::instance->set_a_system(accounting::AccountingSystem::get_instance());
    }
    return InventorySystem::instance.get();
}

InventorySystem::InventorySystem()
{
    this->equipments = std::make_unique<util::baseclass::EquipmentCollection>();
    this->inventories = std::make_unique<util::baseclass::InventoryCollection>();
}

InventorySystem::~InventorySystem()
{
#ifdef DEBUG
    std::cout << "Deleting inventory system" << std::endl;
#endif
}

void InventorySystem::set_a_system(accounting::AccountingSystem *a_system)
{
    this->a_system = a_system;
}

void InventorySystem::add_existing_inventory(Inventory *inv)
{
    this->add_new_item(inv);
}

void InventorySystem::add_existing_asset(Asset *asset)
{
    this->add_new_property(asset);
}

std::unique_ptr<Asset> InventorySystem::get_property(std::string db_code)
{
    std::unique_ptr<util::baseclass::HasTable> from_collection = this->equipments.get()->get_from_database(db_code);
    std::unique_ptr<Asset> to_ret((Asset *)from_collection.release());
    return std::move(to_ret);
}

std::unique_ptr<Inventory> InventorySystem::get_inventory(std::string db_code)
{
    std::unique_ptr<util::baseclass::HasTable> from_collection = this->inventories.get()->get_from_database(db_code);
    std::unique_ptr<Inventory> to_ret((Inventory *)from_collection.release());
    return std::move(to_ret);
}

std::vector<std::unique_ptr<Inventory>> InventorySystem::get_inventory()
{
    std::vector<std::unique_ptr<Inventory>> to_ret;
    std::vector<util::TableCondition> conditions;
    std::vector<std::unique_ptr<util::baseclass::HasTable>> inventories_from_db = this->inventories.get()->get_from_database(conditions);
    for (std::unique_ptr<util::baseclass::HasTable> &inventory_from_db : inventories_from_db)
    {
        std::unique_ptr<Inventory> to_add((Inventory *)inventory_from_db.release());
        to_ret.push_back(std::move(to_add));
    }
    return to_ret;
}

std::vector<std::unique_ptr<Asset>> InventorySystem::get_assets()
{
    std::vector<std::unique_ptr<Asset>> to_ret;
    std::vector<util::TableCondition> conditions;
    std::vector<std::unique_ptr<util::baseclass::HasTable>> equipments_from_db = this->equipments.get()->get_from_database(conditions);
    for (std::unique_ptr<util::baseclass::HasTable> &equipment_from_db : equipments_from_db)
    {
        std::unique_ptr<Asset> to_add((Asset *)equipment_from_db.release());
        to_ret.push_back(std::move(to_add));
    }
    return to_ret;
}

double InventorySystem::sell_sellables(std::shared_ptr<Entry> new_entry)
{
    std::unique_ptr<util::baseclass::HasTable> base = this->inventories->get_from_database(new_entry->get_sellable_db_code());
    std::unique_ptr<Inventory> inventory((Inventory *)base.release());
    std::shared_ptr<SellingEntry> casted_entry = std::static_pointer_cast<SellingEntry>(new_entry);
    double to_ret = inventory->sell_items(casted_entry);
    this->inventories->update_existing_item(inventory.get());
    return to_ret;
}

void InventorySystem::purchase_sellables(std::shared_ptr<Entry> new_entry)
{
    std::unique_ptr<util::baseclass::HasTable> base = this->inventories->get_from_database(new_entry->get_sellable_db_code());
    std::unique_ptr<Inventory> inventory((Inventory *)base.release());
    std::shared_ptr<PurchaseEntry> casted_entry = std::static_pointer_cast<PurchaseEntry>(new_entry);
    inventory->add_purchase(casted_entry);
}

double InventorySystem::sell_properties(std::shared_ptr<Entry> new_entry)
{
    std::unique_ptr<util::baseclass::HasTable> base = this->equipments.get()->get_from_database(
        new_entry->get_properties_db_code());
    std::unique_ptr<Equipment> to_sell((Equipment *)base.release());
    std::shared_ptr<SellingEntry> casted_entry = std::static_pointer_cast<SellingEntry>(new_entry);
    double to_ret = to_sell->sell_items(casted_entry);
    this->equipments->update_existing_item(to_sell.get());
    return to_ret;
}

void InventorySystem::purchase_properties(std::shared_ptr<Entry> new_entry)
{
    std::unique_ptr<util::baseclass::HasTable> base = this->equipments.get()->get_from_database(
        new_entry->get_properties_db_code());
    std::unique_ptr<Equipment> to_sell((Equipment *)base.release());
    std::shared_ptr<PurchaseEntry> casted_entry = std::static_pointer_cast<PurchaseEntry>(new_entry);
    to_sell->add_purchase(casted_entry);
    this->equipments->update_existing_item(to_sell.get());
}

void InventorySystem::add_new_item(Inventory *new_sellable)
{
    this->inventories->insert_new_item(new_sellable);
}

void InventorySystem::add_new_property(Asset *new_depreciable)
{
    this->equipments->insert_new_item(new_depreciable);
}

void InventorySystem::apply_depreciation(std::string asset_db_code)
{
    std::unique_ptr<util::baseclass::HasTable> base = this->equipments->get_from_database(asset_db_code);
    std::unique_ptr<Asset> asset((Asset *)base.release());
    std::unique_ptr<util::Date> now = std::make_unique<util::Date>();
    if ((!asset->get_last_depreciation_date() && now->diff_years_to(asset->get_date_bought()) >= 0) ||
        now->diff_years_to(asset->get_last_depreciation_date()) >= 0)
    {
        return;
    }
    double depreciation_amt_this_year = asset->get_reduced_value_current_year();
    std::unique_ptr<util::Date> new_depreciation_date = std::make_unique<util::Date>();
    asset->set_last_depreciation_date(std::move(new_depreciation_date));
    std::string acct_transaction_title = "Incurred depreciation expense";
    std::unique_ptr<accounting::Transaction> new_transaction =
        util::factory::ApplyDepreciationFactory(std::move(now), acct_transaction_title, asset_db_code, depreciation_amt_this_year)
            .create_transaction();
    this->a_system->add_transaction(new_transaction.get());
}

void InventorySystem::apply_all_depreciation()
{
    std::vector<util::TableCondition> conditions;
    std::vector<std::unique_ptr<util::baseclass::HasTable>> all = this->equipments->get_from_database(conditions);
    for (std::unique_ptr<util::baseclass::HasTable> &single : all)
    {
        this->apply_depreciation(single->get_db_code());
    }
}

std::string InventorySystem::to_string()
{
    std::string to_ret = "";
    /*for (auto it = this->sellables.begin(); it != this->sellables.end(); it++)
    {
        to_ret += it->second->to_string();
    }
    for (auto it = this->assets.begin(); it != this->assets.end(); it++)
    {
        to_ret += it->second->to_string();
    }*/
    return to_ret;
}