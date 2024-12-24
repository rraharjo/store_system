#include <string>
#include <sstream>
#include "inventory/assets/items.hpp"
#include "util/database/tables.hpp"
#include "util/database/db.hpp"

#ifndef INVENTORYINVENTORY_HPP
#define INVENTORYINVENTORY_HPP

namespace inventory
{
    class Inventory : public Item
    {
    private:
        static util::Table *class_table;

        double selling_price;
        // tax

        void add_existing_purchase_entry(PurchaseEntry *) override;

    protected:
        std::vector<std::string> get_insert_parameter() override;

        std::vector<std::string> get_update_parameter() override;

    public:
        static std::vector<Inventory *> generate_from_database();

        void insert_to_db() override;

        void update_to_db() override;

        Inventory(std::string, std::string, std::string, double);

        Inventory(std::string item_code, std::string name, double selling_price);

        double sell_items(SellingEntry *entry) override;

        void add_purchase(PurchaseEntry *entry) override;

        double get_selling_price();

        void set_selling_price(double new_price);

        std::string to_string();
    };

};

#endif