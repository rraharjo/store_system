#include "./assets/sellable.hpp"
#include "./assets/depreciable.hpp"
#ifndef INVENTORYSYSTEM_HPP
#define INVENTORYSYSTEM_HPP
namespace inventory
{
    class InventorySystem
    {
    private:
        static InventorySystem *instance;
        std::map<std::string, Sellable *> sellables;
        std::map<std::string, Depreciable *> properties; 
        InventorySystem();

    public:
    
        static InventorySystem *getInstance();

        Depreciable *getProperty(std::string dbCode);

        double sellSellables(Entry *newEntry); // return the COGS

        void purchaseSellables(Entry *newEntry);
        // void purchaseItem(int itemDBCode, int qty);

        double sellProperties(Entry *newEntry);

        void purchaseProperties(Entry *newEntry);

        void addNewItem(Sellable *newSellable);

        void addNewProperty(Depreciable *newProperty);

        std::string to_string();
    };
};
#endif