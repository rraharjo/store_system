#include <chrono>
#include <deque>
#include <algorithm>
#include "inventory/transaction/transaction_history.hpp"

using namespace inventory;

/*************************************Transaction History (parent class)*************************************/
TransactionHistory::TransactionHistory()
{
    this->transactionHistory = {};
}

void TransactionHistory::addEntry(TransactionEntry *entry)
{
    this->transactionHistory.push_back(entry);
    auto end = this->transactionHistory.end() - 1;
    while (end != this->transactionHistory.begin() && (*end)->purchaseDate < (*(end - 1))->purchaseDate)
    {
        std::iter_swap(end - 1, end);
        end -= 1;
    }
}

/*************************************Purchase History*************************************/
PurchaseHistory::PurchaseHistory() : TransactionHistory()
{
    this->setTable();
}

void PurchaseHistory::setTable()
{
    this->table = util::PurchaseEntryTable::getInstance();
}

double PurchaseHistory::sellItemFirstIn(int qty)
{
    double toRet = 0.0;
    while (qty > 0)
    {
        TransactionEntry *earliest = this->transactionHistory.front();
        int toSubtract = std::min(qty, earliest->qty);
        qty -= toSubtract;
        earliest->qty -= toSubtract;
        toRet += toSubtract * (earliest->price);
        if (!earliest->qty)
        {
            this->transactionHistory.pop_front();
        }
    }
    return toRet;
}

double PurchaseHistory::sellItemLastIn(int qty)
{
    double toRet = 0.0;
    while (qty > 0)
    {
        TransactionEntry *latest = this->transactionHistory.back();
        int toSubtract = std::min(qty, latest->qty);
        qty -= toSubtract;
        latest->qty -= toSubtract;
        toRet += toSubtract * (latest->price);
        if (!latest->qty)
        {
            this->transactionHistory.pop_back();
        }
    }
    return toRet;
}
/*************************************Selling History*************************************/
SellingHistory::SellingHistory() : TransactionHistory()
{
    this->setTable();
}

void SellingHistory::setTable()
{
    this->table = util::SellingEntryTable::getInstance();
}