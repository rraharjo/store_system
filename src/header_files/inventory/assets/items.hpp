#include <string>
#include "util/class/base_class.hpp"
#include "inventory/transaction/transaction_history.hpp"
#ifndef ITEMINVENTORY_HPP
#define ITEMINVENTORY_HPP
namespace inventory
{
    class Item : public util::baseclass::HasTable
    {
    protected:
        std::string name;
        std::string item_code;
        PurchaseHistory *purchase_history;
        SellingHistory *selling_history;
        int qty;

        Item(std::string name, std::string item_code);

        virtual void add_existing_purchase_entry(PurchaseEntry *);

        void add_existing_selling_entry(SellingEntry *);

    public:
        std::string get_name();
        std::string get_item_code();

        int get_qty();

        virtual double sell_items(SellingEntry *entry) = 0;

        virtual void add_purchase(PurchaseEntry *entry) = 0;
    };
};

#endif