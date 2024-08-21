#include "./assets/sellable.hpp"
#include "./assets/depreciable.hpp"
#include "./transaction/transaction.hpp"
#ifndef INVENTORYSYSTEM_HPP
#define INVENTORYSYSTEM_HPP
namespace inventory{
    class InventorySystem{
        private:
            std::vector<PurchaseTransaction*> purchaseTransactions;
            std::vector<Sellable*> sellables;
        public:
            InventorySystem();

            int itemExist(int itemCode);

            double sellItem(Entry* newEntry);//return the COGS

            void purchaseItem(Entry* newEntry);
            //void purchaseItem(int itemDBCode, int qty);

            void addNewItem(Sellable *newSellable);

            std::string to_string();
    };

    int binSearch(std::vector<Sellable *> sellables, int dbCode);
};
#endif