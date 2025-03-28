#ifndef ITEMINVENTORY_HPP
#define ITEMINVENTORY_HPP
#include <memory>
#include <string>
#include "util/class/base_class.hpp"
#include "inventory/transaction/transaction_history.hpp"

namespace inventory
{
    class Item : public util::baseclass::HasTable
    {
    protected:
        std::string name;
        std::string item_code;
        std::unique_ptr<PurchaseHistory> purchase_history;
        std::unique_ptr<SellingHistory> selling_history;
        int qty;

        Item(util::enums::PrimaryKeyPrefix primary_key_prefix, std::string name, std::string item_code);

        /* Item takes over the ownership */
        virtual void add_existing_purchase_entry(std::unique_ptr<PurchaseEntry> existing_entry);

        void add_existing_selling_entry(std::unique_ptr<SellingEntry> existing_entry);

        std::vector<std::shared_ptr<PurchaseEntry>> get_purchase_entries();

        std::vector<std::shared_ptr<SellingEntry>> get_selling_entries();

    public:
        virtual ~Item();

        std::string get_name();

        std::string get_item_code();

        int get_qty();

        virtual double sell_items(std::shared_ptr<SellingEntry> entry) = 0;

        virtual void add_purchase(std::shared_ptr<PurchaseEntry> entry) = 0;
    };
};

#endif