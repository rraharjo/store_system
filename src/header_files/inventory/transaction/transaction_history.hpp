#include <chrono>
#include <deque>
#include <algorithm>
#include "inventory/transaction/entry.hpp"
#include "util/class/base_class.hpp"
#include "util/database/tables.hpp"
#ifndef INVENTORYPURCHASEHISTORY_HPP
#define INVENTORYPURCHASEHISTORY_HPP
// FIX THIS FIRST (7/28/2024)
namespace inventory
{
    /*************************************Transaction History (parent class)*************************************/
    class TransactionHistory
    {
    protected:
        std::deque<Entry *> entries;

        TransactionHistory();

    public:
        void add_entry(Entry *entry);
    };

    /*************************************Purchase History*************************************/
    class PurchaseHistory : public TransactionHistory
    {
    private:
        bool fifo;

    public:
        PurchaseHistory();

        double sell_item_first_in(int qty);

        double sell_item_last_in(int qty);
    };

    /*************************************Selling History*************************************/
    class SellingHistory : public TransactionHistory
    {
    public:
        SellingHistory();
    };
};

#endif