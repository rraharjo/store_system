#include <deque>
#include <string>
#include "inventory/assets/items.hpp"
#include "inventory/transaction/transaction_history.hpp"
#include "util/database/tables.hpp"
#include "util/database/db.hpp"

#ifndef SELLABLEINVENTORY_HPP
#define SELLABLEINVENTORY_HPP

namespace inventory
{
    class Sellable : public Item
    {
    private:
        double sellingPrice;
        int qty;
        PurchaseHistory *purchaseHistory;
        SellingHistory *sellingHistory;
        // tax

    protected:
        void setTable() override;

        std::vector<std::string> getInsertParameter() override;

    public:
        Sellable(std::string name, std::string itemCode, double sellingPrice);

        double getSellingPrice();

        void setSellingPrice(double newPrice);

        double sellItems(SellingEntry *entry);

        void addPurchase(PurchaseEntry *entry);

        std::string to_string();
    };

};

#endif