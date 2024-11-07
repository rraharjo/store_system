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

        virtual void addExistingPurchaseEntry(PurchaseEntry *);

        void addExistingSellingEntry(SellingEntry *);

    public:
        std::string getName();
        std::string getItemCode();

        int getQty();

        virtual double sellItems(SellingEntry *entry) = 0;

        virtual void addPurchase(PurchaseEntry *entry) = 0;
    };
};

#endif