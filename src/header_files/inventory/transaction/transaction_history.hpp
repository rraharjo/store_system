#include <chrono>
#include <deque>
#include <algorithm>
#include "../constants/structs.hpp"
#include "util/class/base_class.hpp"
#include "util/database/tables.hpp"
#ifndef INVENTORYPURCHASEHISTORY_HPP
#define INVENTORYPURCHASEHISTORY_HPP
// FIX THIS FIRST (7/28/2024)
namespace inventory
{
    /*************************************Transaction History (parent class)*************************************/
    class TransactionHistory : public util::baseclass::HasTable
    {
    protected:
        std::deque<TransactionEntry *> transactionHistory;

        TransactionHistory();

    public:
        void addEntry(TransactionEntry *entry);
    };

    /*************************************Purchase History*************************************/
    class PurchaseHistory : public TransactionHistory
    {
    private:
        bool fifo;

    protected:
        void setTable() override;

    public:
        PurchaseHistory();

        double sellItemFirstIn(int qty);

        double sellItemLastIn(int qty);
    };

    /*************************************Selling History*************************************/
    class SellingHistory : public TransactionHistory
    {
    protected:
        void setTable() override;

    public:
        SellingHistory();
    };
};

#endif