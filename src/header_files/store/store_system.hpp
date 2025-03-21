#include <memory>
#include "store/transaction/transaction.hpp"
#include "util/class/purchase_transaction_collection.hpp"
#include "util/class/selling_transaction_collection.hpp"    
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
        static std::shared_ptr<StoreSystem> instance;
        std::unique_ptr<util::baseclass::PurchaseTransactionCollection> purchase_transactions;
        std::unique_ptr<util::baseclass::SellingTransactionCollection> selling_transactions;
        std::shared_ptr<accounting::AccountingSystem> a_system;
        std::shared_ptr<inventory::InventorySystem> i_system;

        StoreSystem();

    public:
        static std::shared_ptr<StoreSystem> get_instance();

        ~StoreSystem();

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