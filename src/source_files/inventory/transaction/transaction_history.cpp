#include <chrono>
#include <deque>
#include <algorithm>
#include "inventory/transaction/transaction_history.hpp"

using namespace inventory;

/*************************************Transaction History (parent class)*************************************/
TransactionHistory::TransactionHistory()
{
    this->entries = {};
}

void TransactionHistory::addEntry(Entry *entry)
{
    this->entries.push_back(entry);
    auto end = this->entries.end() - 1;
    while (end != this->entries.begin() && (*end)->getTransactionDate() < (*(end - 1))->getTransactionDate())
    {
        std::iter_swap(end - 1, end);
        end -= 1;
    }
}

/*************************************Purchase History*************************************/
PurchaseHistory::PurchaseHistory() : TransactionHistory()
{
}

double PurchaseHistory::sellItemFirstIn(int qty)
{
    double toRet = 0.0;
    while (qty > 0)
    {
        PurchaseEntry *earliest = (PurchaseEntry*) this->entries.front();
        int toSubtract = std::min(qty, earliest->getQty());
        qty -= toSubtract;
        earliest->setAvailableQty(earliest->getQty() - toSubtract);
        toRet += toSubtract * (earliest->getPrice());
        earliest->updateToDB();
        if (!earliest->getAvailableQty())
        {
            this->entries.pop_front();
        }
    }
    return toRet;
}

double PurchaseHistory::sellItemLastIn(int qty)
{
    double toRet = 0.0;
    while (qty > 0)
    {
        PurchaseEntry *earliest = (PurchaseEntry*) this->entries.back();
        int toSubtract = std::min(qty, earliest->getQty());
        qty -= toSubtract;
        earliest->setAvailableQty(earliest->getQty() - toSubtract);
        toRet += toSubtract * (earliest->getPrice());
        earliest->updateToDB();
        if (!earliest->getAvailableQty())
        {
            this->entries.pop_back();
        }
    }
    return toRet;
}
/*************************************Selling History*************************************/
SellingHistory::SellingHistory() : TransactionHistory()
{
}