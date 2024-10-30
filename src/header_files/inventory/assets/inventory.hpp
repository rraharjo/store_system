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

        double sellingPrice;
        // tax

        void addExistingPurchaseEntry(PurchaseEntry *) override;

    protected:
        std::vector<std::string> getInsertParameter() override;

        std::vector<std::string> getUpdateParameter() override;

    public:
        static std::vector<Inventory *> generateFromDatabase();

        void insertToDB() override;

        void updateToDB() override;

        Inventory(std::string, std::string, std::string, double);

        Inventory(std::string itemCode, std::string name, double sellingPrice);

        double sellItems(SellingEntry *entry) override;

        void addPurchase(PurchaseEntry *entry) override;

        double getSellingPrice();

        void setSellingPrice(double newPrice);

        std::string to_string();
    };

};

#endif