#include "./assets/inventory.hpp"
#include "./assets/equipment.hpp"
#ifndef INVENTORYSYSTEM_HPP
#define INVENTORYSYSTEM_HPP
namespace inventory
{
    class InventorySystem
    {
    private:
        static InventorySystem *instance;
        std::map<std::string, Inventory *> sellables;
        std::map<std::string, Asset *> assets; 
        InventorySystem();

    public:
    
        static InventorySystem *getInstance();

        Asset *getProperty(std::string dbCode);

        double sellSellables(Entry *newEntry); // return the COGS

        void purchaseSellables(Entry *newEntry);

        double sellProperties(Entry *newEntry);

        void purchaseProperties(Entry *newEntry);

        void addNewItem(Inventory *newSellable);

        void addNewProperty(Asset *newProperty);

        std::string to_string();
    };
};
#endif