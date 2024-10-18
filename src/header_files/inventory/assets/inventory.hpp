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
        static util::Table *classTable;
        static int nextItemCode;
        double sellingPrice;
        // tax

    protected:
        std::vector<std::string> getInsertParameter() override;

        std::string createDBCode() override;

    public:
        void insertToDB() override;

        void updateToDB() override;
        Inventory(std::string name, std::string itemCode, double sellingPrice);

        double sellItems(SellingEntry *entry) override;

        void addPurchase(PurchaseEntry *entry) override;

        double getSellingPrice();

        void setSellingPrice(double newPrice);

        std::string to_string();
    };

};

#endif