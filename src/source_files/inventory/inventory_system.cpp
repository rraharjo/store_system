#include "inventory/inventory_system.hpp"
using namespace inventory;

InventorySystem *InventorySystem::instance = NULL;

InventorySystem *InventorySystem::get_instance()
{
    if (InventorySystem::instance == NULL)
    {
        InventorySystem::instance = new InventorySystem();
        InventorySystem::instance->set_a_system(accounting::AccountingSystem::get_instance());
    }
    return InventorySystem::instance;
}

InventorySystem::InventorySystem()
{
    this->equipments = std::unique_ptr<util::baseclass::EquipmentCollection>(new util::baseclass::EquipmentCollection());
    this->inventories = std::unique_ptr<util::baseclass::InventoryCollection>(new util::baseclass::InventoryCollection());
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

Asset *InventorySystem::get_property(std::string db_code)
{
    return (Asset *)this->equipments.get()->get_from_database(db_code);
}

Inventory *InventorySystem::get_inventory(std::string db_code)
{
    return (Inventory *)this->inventories.get()->get_from_database(db_code);
}

std::vector<Inventory *> InventorySystem::get_inventory()
{
    std::vector<Inventory *> to_ret;
    std::vector<util::TableCondition> conditions;
    std::vector<util::baseclass::HasTable *> inventories_from_db = this->inventories.get()->get_from_database(conditions);
    for (util::baseclass::HasTable *inventory_from_db : inventories_from_db)
    {
        to_ret.push_back((Inventory *)inventory_from_db);
    }
    return to_ret;
}

std::vector<Asset *> InventorySystem::get_assets()
{
    std::vector<Asset *> to_ret;
    std::vector<util::TableCondition> conditions;
    std::vector<util::baseclass::HasTable *> equipments_from_db = this->equipments.get()->get_from_database(conditions);
    for (util::baseclass::HasTable *equipment_from_db : equipments_from_db)
    {
        to_ret.push_back((Asset *)equipment_from_db);
    }
    return to_ret;
}

double InventorySystem::sell_sellables(Entry *new_entry)
{
    Inventory *inventory = (Inventory *)this->inventories->get_from_database(new_entry->get_db_code());
    return inventory->sell_items((SellingEntry *)new_entry);
}

void InventorySystem::purchase_sellables(Entry *new_entry)
{
    Inventory *inventory = (Inventory *)this->inventories->get_from_database(new_entry->get_sellable_db_code());
    inventory->add_purchase((PurchaseEntry *)new_entry);
}

double InventorySystem::sell_properties(Entry *new_entry)
{
    Equipment *to_sell = (Equipment *)this->equipments.get()->get_from_database(
        new_entry->get_properties_db_code());
    return to_sell->sell_items((SellingEntry *)new_entry);
}

void InventorySystem::purchase_properties(Entry *new_entry)
{

    Equipment *to_sell = (Equipment *)this->equipments.get()->get_from_database(
        new_entry->get_properties_db_code());
    to_sell->add_purchase((PurchaseEntry *)new_entry);
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
    Asset *asset = (Asset *)this->equipments->get_from_database(asset_db_code);
    util::Date *now = new util::Date();
    if ((!asset->get_last_depreciation_date() && now->diff_years_to(asset->get_date_bought()) >= 0) ||
        now->diff_years_to(asset->get_last_depreciation_date()) >= 0)
    {
        delete now;
        return;
    }
    double depreciation_amt_this_year = asset->get_reduced_value_current_year();
    util::Date *new_depreciation_date = new util::Date();
    delete asset->get_last_depreciation_date();
    asset->set_last_depreciation_date(new_depreciation_date);
    std::string acct_transaction_title = "Incurred depreciation expense";
    accounting::Transaction *new_transaction =
        util::factory::ApplyDepreciationFactory(now, acct_transaction_title, asset_db_code, depreciation_amt_this_year)
            .create_transaction();
    this->a_system->add_transaction(new_transaction);
}

void InventorySystem::apply_all_depreciation()
{
    std::vector<util::TableCondition> conditions;
    std::vector<util::baseclass::HasTable *> all = this->equipments->get_from_database(conditions);
    for (util::baseclass::HasTable *single : all)
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