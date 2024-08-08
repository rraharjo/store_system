#include <deque>
#include "./items.hpp"
#include "../transaction/purchase_history.hpp"
#include "../constants/structs.hpp"
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
        PurchaseHistory *history;
        // tax

    public:
        Sellable(std::string name, std::string itemCode, double sellingPrice);

        int getDBCode();

        double getSellingPrice();

        void setSellingPrice(double newPrice);

        double removeItem(int qty);

        void addPurchase(TransactionEntry *entry);

        std::string to_string();
    };

};

#endif