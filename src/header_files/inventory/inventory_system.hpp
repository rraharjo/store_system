#include "./assets/inventory.hpp"
#include "./assets/equipment.hpp"
#include "accounting/accounting_system.hpp"
#include "util/factory/transaction_factory.hpp"
#ifndef INVENTORYSYSTEM_HPP
#define INVENTORYSYSTEM_HPP
namespace inventory
{
    class InventorySystem
    {
    private:
        static InventorySystem *instance;
        accounting::AccountingSystem *a_system;
        std::map<std::string, Inventory *> sellables;
        std::map<std::string, Asset *> assets; 

        InventorySystem();

        void add_existing_asset(Asset *);

        void add_existing_inventory(Inventory *);
    public:
    
        static InventorySystem *get_instance();

        Asset *get_property(std::string db_code);

        Inventory *get_inventory(std::string db_code);

        std::vector<Inventory *> get_inventory();

        void set_a_system(accounting::AccountingSystem *a_system);

        double sell_sellables(Entry *new_entry); // return the COGS

        void purchase_sellables(Entry *new_entry);

        double sell_properties(Entry *new_entry);

        void purchase_properties(Entry *new_entry);

        void add_new_item(Inventory *new_sellable);

        void add_new_property(Asset *new_property);

        void apply_depreciation(std::string asset_db_code);

        void apply_all_depreciation();

        std::string to_string();
    };
};
#endif