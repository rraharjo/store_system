#include "inventory/inventory_system.hpp"
using namespace inventory;

std::shared_ptr<InventorySystem> InventorySystem::instance = NULL;

std::shared_ptr<InventorySystem> InventorySystem::get_instance()
{
    if (InventorySystem::instance.get() == NULL)
    {
        InventorySystem::instance.reset(new InventorySystem());
        InventorySystem::instance->set_a_system(accounting::AccountingSystem::get_instance());
    }
    return InventorySystem::instance;
}

InventorySystem::InventorySystem()
{
    this->equipments = std::unique_ptr<util::baseclass::EquipmentCollection>(new util::baseclass::EquipmentCollection());
    this->inventories = std::unique_ptr<util::baseclass::InventoryCollection>(new util::baseclass::InventoryCollection());
}

InventorySystem::~InventorySystem()
{
#ifdef DEBUG
    std::cout << "Deleting inventory system" << std::endl;
#endif
}

void InventorySystem::set_a_system(std::shared_ptr<accounting::AccountingSystem> a_system)
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

double InventorySystem::sell_sellables(std::shared_ptr<Entry> new_entry)
{
    Inventory *inventory = (Inventory *)this->inventories->get_from_database(new_entry->get_sellable_db_code());
    std::shared_ptr<SellingEntry> casted_entry = std::static_pointer_cast<SellingEntry>(new_entry);
    double to_ret = inventory->sell_items(casted_entry);
    this->inventories->update_existing_item(inventory);
    delete inventory;
    return to_ret;
}

void InventorySystem::purchase_sellables(std::shared_ptr<Entry> new_entry)
{
    Inventory *inventory = (Inventory *)this->inventories->get_from_database(new_entry->get_sellable_db_code());
    std::shared_ptr<PurchaseEntry> casted_entry = std::static_pointer_cast<PurchaseEntry>(new_entry);
    inventory->add_purchase(casted_entry);
    delete inventory;
}

double InventorySystem::sell_properties(std::shared_ptr<Entry> new_entry)
{
    Equipment *to_sell = (Equipment *)this->equipments.get()->get_from_database(
        new_entry->get_properties_db_code());
    std::shared_ptr<SellingEntry> casted_entry = std::static_pointer_cast<SellingEntry>(new_entry);
    double to_ret = to_sell->sell_items(casted_entry);
    this->equipments->update_existing_item(to_sell);
    delete to_sell;
    return to_ret;
}

void InventorySystem::purchase_properties(std::shared_ptr<Entry> new_entry)
{
    Equipment *to_sell = (Equipment *)this->equipments.get()->get_from_database(
        new_entry->get_properties_db_code());
    std::shared_ptr<PurchaseEntry> casted_entry = std::static_pointer_cast<PurchaseEntry>(new_entry);
    to_sell->add_purchase(casted_entry);
    this->equipments->update_existing_item(to_sell);
    delete to_sell;
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
    asset->set_last_depreciation_date(new_depreciation_date);
    std::string acct_transaction_title = "Incurred depreciation expense";
    accounting::Transaction *new_transaction =
        util::factory::ApplyDepreciationFactory(now, acct_transaction_title, asset_db_code, depreciation_amt_this_year)
            .create_transaction();
    this->a_system->add_transaction(new_transaction);
    delete asset;
    delete new_transaction;
}

void InventorySystem::apply_all_depreciation()
{
    std::vector<util::TableCondition> conditions;
    std::vector<util::baseclass::HasTable *> all = this->equipments->get_from_database(conditions);
    for (util::baseclass::HasTable *single : all)
    {
        this->apply_depreciation(single->get_db_code());
        delete single;
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