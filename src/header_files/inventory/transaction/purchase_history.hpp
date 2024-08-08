#include <chrono>
#include <deque>
#include <algorithm>
#include "../constants/structs.hpp"
#ifndef INVENTORYPURCHASEHISTORY_HPP
#define INVENTORYPURCHASEHISTORY_HPP

namespace inventory
{
    class PurchaseHistory
    {
    private:
        std::deque<TransactionEntry *> purchaseHistories;
        bool fifo;

    public:
        PurchaseHistory();

        void addEntry(TransactionEntry *entry);

        double sellItemFirstIn(int qty);

        double sellItemLastIn(int qty);
    };
};

#endif