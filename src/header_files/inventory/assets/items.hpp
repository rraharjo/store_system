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
        std::string itemCode;
        PurchaseHistory *purchaseHistory;
        SellingHistory *sellingHistory;
        int qty;
        Item(std::string name, std::string itemCode);

    public:
        std::string getName();
        std::string getItemCode();
        int getQty();
        double sellItems(SellingEntry *entry);
        void addPurchase(PurchaseEntry *entry);
    };
};

#endif