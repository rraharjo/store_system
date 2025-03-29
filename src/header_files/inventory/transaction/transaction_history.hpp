#ifndef INVENTORYPURCHASEHISTORY_HPP
#define INVENTORYPURCHASEHISTORY_HPP
#include <chrono>
#include <deque>
#include <algorithm>
#include "inventory/transaction/entry.hpp"
#include "util/class/base_class.hpp"
#include "util/database/tables.hpp"

namespace inventory
{
    class TransactionHistory
    {
    protected:
        std::deque<std::shared_ptr<Entry>> entries;

        TransactionHistory();

    public:
        virtual ~TransactionHistory();

        void add_entry(std::shared_ptr<Entry> entry);

        std::vector<std::shared_ptr<Entry>> get_entries();
    };

    class PurchaseHistory : public TransactionHistory
    {
    private:
        bool fifo;

    public:
        PurchaseHistory();

        ~PurchaseHistory();

        double sell_item_first_in(int qty);

        double sell_item_last_in(int qty);
    };

    class SellingHistory : public TransactionHistory
    {
    public:
        SellingHistory();

        ~SellingHistory();
    };
};

#endif