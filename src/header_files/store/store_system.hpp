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
        std::vector<PurchaseTransaction *> purchase_transactions;
        std::vector<SellingTransaction *> selling_transactions;
        accounting::AccountingSystem *a_system;
        inventory::InventorySystem *i_system;

        StoreSystem();

    public:
        static StoreSystem *get_instance();

        void sell_item(SellingTransaction *sell_transaction);

        void buy_item(PurchaseTransaction *purchase_transaction);

        void capitalize_asset(PurchaseTransaction *purchase_transaction);

        void dispose_asset(SellingTransaction *sell_transaction);

        void add_item(inventory::Inventory *new_sellable);

        void add_property(inventory::Equipment *new_property);

        void end_year_adjustment();

        std::vector<inventory::Inventory *> get_inventory();

        inventory::Inventory *get_inventory(std::string db_code);

        std::vector<inventory::Asset *> get_assets();

        std::string to_string_inv();

        std::string to_string();
    };
}
#endif