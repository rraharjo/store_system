#include "inventory/inventory_system.hpp"
using namespace inventory;

InventorySystem *InventorySystem::instance = NULL;

InventorySystem *InventorySystem::get_instance()
{
    if (InventorySystem::instance == NULL)
    {
        InventorySystem::instance = new InventorySystem();
        std::vector<Inventory *> inventories = Inventory::generate_from_database();
        for (Inventory *inventory : inventories)
        {
            InventorySystem::instance->sellables[inventory->get_db_code()] = inventory;
        }
        std::vector<Asset *> equipments = Equipment::generate_from_database();
        for (Asset *equipment : equipments)
        {
            InventorySystem::instance->assets[equipment->get_db_code()] = equipment;
        }
        InventorySystem::instance->set_a_system(accounting::AccountingSystem::get_instance());
    }
    return InventorySystem::instance;
}

InventorySystem::InventorySystem()
{
    this->sellables = {};
    this->assets = {};
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
    return this->assets[db_code];
}

Inventory *InventorySystem::get_inventory(std::string db_code)
{
    return this->sellables[db_code];
}

std::vector<Inventory *> InventorySystem::get_inventory()
{
    std::vector<Inventory *> to_ret;
    for (std::map<std::string, Inventory *>::iterator it = this->sellables.begin(); it != this->sellables.end(); it++)
    {
        to_ret.push_back(it->second);
    }
    return to_ret;
}

double InventorySystem::sell_sellables(Entry *new_entry)
{
    if (!this->sellables[new_entry->get_sellable_db_code()])
    {
        return -1;
    }
    return this->sellables[new_entry->get_sellable_db_code()]->sell_items((SellingEntry *)new_entry);
}

void InventorySystem::purchase_sellables(Entry *new_entry)
{
    if (!this->sellables[new_entry->get_sellable_db_code()])
    {
        return;
    }
    this->sellables[new_entry->get_sellable_db_code()]->add_purchase((PurchaseEntry *)new_entry);
}

double InventorySystem::sell_properties(Entry *new_entry)
{
    if (!this->assets[new_entry->get_properties_db_code()])
    {
        return -1;
    }
    return this->assets[new_entry->get_properties_db_code()]->sell_items((SellingEntry *)new_entry);
}

void InventorySystem::purchase_properties(Entry *new_entry)
{
    if (!this->assets[new_entry->get_properties_db_code()])
    {
        return;
    }
    this->assets[new_entry->get_properties_db_code()]->add_purchase((PurchaseEntry *)new_entry);
}

void InventorySystem::add_new_item(Inventory *new_sellable)
{
    if (this->sellables.find(new_sellable->get_db_code()) == this->sellables.end())
    {
        this->sellables[new_sellable->get_db_code()] = new_sellable;
    }
}

void InventorySystem::add_new_property(Asset *new_depreciable)
{
    if (this->assets.find(new_depreciable->get_db_code()) == this->assets.end())
    {
        this->assets[new_depreciable->get_db_code()] = new_depreciable;
    }
}

void InventorySystem::apply_depreciation(std::string asset_db_code)
{
    Asset *asset = this->assets[asset_db_code];
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
    for (auto it = this->assets.begin(); it != this->assets.end(); it++)
    {
        this->apply_depreciation(it->first);
    }
}

std::string InventorySystem::to_string()
{
    std::string to_ret = "";
    for (auto it = this->sellables.begin(); it != this->sellables.end(); it++)
    {
        to_ret += it->second->to_string();
    }
    for (auto it = this->assets.begin(); it != this->assets.end(); it++)
    {
        to_ret += it->second->to_string();
    }
    return to_ret;
}