#include "store/transaction/transaction.hpp"
#include "inventory/inventory_system.hpp"
#include "accounting/accounting_system.hpp"
#include "util/factory/transaction_factory.hpp"
#ifndef STORE_SYSTEM_HPP
#define STORE_SYSTEM_HPP
namespace store
{
    class StoreSystem
    {
    private:
        static StoreSystem *instance;
        std::vector<PurchaseTransaction *> purchaseTransactions;
        std::vector<SellingTransaction *> sellingTransactions;
        accounting::AccountingSystem *aSystem;
        inventory::InventorySystem *iSystem;

        StoreSystem();

    public:
        static StoreSystem *getInstance();

        void sellItem(SellingTransaction *sellTransaction);

        void buyItem(PurchaseTransaction *purchaseTransaction);

        void capitalizeAsset(PurchaseTransaction *purchaseTransaction);

        void disposeAsset(SellingTransaction *sellTransaction);

        void addItem(inventory::Inventory *newSellable);

        void addProperty(inventory::Equipment *newProperty);

        void endYearAdjustment();

        inventory::Inventory *getInventory(std::string dbCode);

        std::string toStringInv();

        std::string toString();
    };
}
#endif