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

        double sellItem(Entry *newEntry); // return the COGS

        void purchaseItem(Entry *newEntry);
        // void purchaseItem(int itemDBCode, int qty);

        void addNewItem(Sellable *newSellable);

        void addNewProperty(Depreciable *newProperty);

        Depreciable *disposeProperty(std::string dbCode);

        std::string to_string();
    };
};
#endif