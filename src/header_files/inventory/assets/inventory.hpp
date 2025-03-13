#include <string>
#include <sstream>
#include "inventory/assets/items.hpp"
#include "util/database/tables.hpp"
#include "util/database/db.hpp"

#ifndef INVENTORYINVENTORY_HPP
#define INVENTORYINVENTORY_HPP

namespace util
{
    namespace baseclass
    {
        class InventoryCollection;
    }
}

namespace inventory
{
    class Inventory : public Item
    {
    private:
        double selling_price;
        // tax

        void add_existing_purchase_entry(PurchaseEntry *) override;

    public:
        Inventory(std::string, std::string, std::string, double);

        Inventory(std::string item_code, std::string name, double selling_price);

        double sell_items(SellingEntry *entry) override;

        void add_purchase(PurchaseEntry *entry) override;

        double get_selling_price();

        void set_selling_price(double new_price);

        std::string to_string();

        friend class util::baseclass::InventoryCollection;
    };

};

#endif