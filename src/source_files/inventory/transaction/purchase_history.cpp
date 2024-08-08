#include <chrono>
#include <deque>
#include <algorithm>
#include "inventory/transaction/purchase_history.hpp"


using namespace inventory;

PurchaseHistory::PurchaseHistory()
{
    this->purchaseHistories = {};
}

void PurchaseHistory::addEntry(TransactionEntry *entry)
{
    this->purchaseHistories.push_back(entry);
    auto end = this->purchaseHistories.end() - 1;
    while (end != this->purchaseHistories.begin() && (*end)->purchaseDate < (*(end - 1))->purchaseDate)
    {
        std::iter_swap(end - 1, end);
        end -= 1;
    }
}

double PurchaseHistory::sellItemFirstIn(int qty)
{
    double toRet = 0.0;
    while (qty > 0)
    {
        TransactionEntry *earliest = this->purchaseHistories.front();
        int toSubtract = std::min(qty, earliest->qty);
        qty -= toSubtract;
        earliest->qty -= toSubtract;
        toRet += toSubtract * (earliest->price);
        if (!earliest->qty)
        {
            this->purchaseHistories.pop_front();
        }
    }
    return toRet;
}

double PurchaseHistory::sellItemLastIn(int qty)
{
    double toRet = 0.0;
    while (qty > 0)
    {
        TransactionEntry *latest = this->purchaseHistories.back();
        int toSubtract = std::min(qty, latest->qty);
        qty -= toSubtract;
        latest->qty -= toSubtract;
        toRet += toSubtract * (latest->price);
        if (!latest->qty)
        {
            this->purchaseHistories.pop_back();
        }
    }
    return toRet;
}
