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
        accounting::AccountingSystem *aSystem;
        std::map<std::string, Inventory *> sellables;
        std::map<std::string, Asset *> assets; 

        InventorySystem();

        void addExistingAsset(Asset *);

        void addExistingInventory(Inventory *);
    public:
    
        static InventorySystem *getInstance();

        Asset *getProperty(std::string dbCode);

        Inventory *getInventory(std::string dbCode);

        void setASystem(accounting::AccountingSystem *aSystem);

        double sellSellables(Entry *newEntry); // return the COGS

        void purchaseSellables(Entry *newEntry);

        double sellProperties(Entry *newEntry);

        void purchaseProperties(Entry *newEntry);

        void addNewItem(Inventory *newSellable);

        void addNewProperty(Asset *newProperty);

        void applyDepreciation(std::string assetDBCode);

        void applyAllDepreciation();

        std::string to_string();
    };
};
#endif