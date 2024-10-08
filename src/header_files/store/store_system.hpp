#include "store/transaction/transaction.hpp"
#include "inventory/inventory_system.hpp"
#include "accounting/accounting_system.hpp"
#include "util/factory/transaction_factory.hpp"
#ifndef STORE_SYSTEM_HPP
#define STORE_SYSTEM_HPP
namespace store {
    class StoreSystem {
        private:
            std::vector<PurchaseTransaction*> purchaseTransactions;
            std::vector<SellingTransaction*> sellingTransactions;
            accounting::AccountingSystem* aSystem;
            inventory::InventorySystem* iSystem;
        

        public:
            StoreSystem();

            void sellItem(SellingTransaction* sellTransaction);

            void buyItem(PurchaseTransaction* purchaseTransaction);

            void buyProperty(PurchaseTransaction* purchaseTransaction);
            
            void disposeProperty(SellingTransaction* sellTransaction);

            void addItem(inventory::Sellable *newSellable);

            void addProperty(inventory::Depreciable *newProperty);

            std::string toString();
    };
}
#endif