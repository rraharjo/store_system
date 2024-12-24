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

void TransactionHistory::add_entry(Entry *entry)
{
    this->entries.push_back(entry);
    //entry->insert_to_db();
    /*auto end = this->entries.end() - 1;
    while (end != this->entries.begin() && (*end)->get_transaction_date() < (*(end - 1))->get_transaction_date())
    {
        std::iter_swap(end - 1, end);
        end -= 1;
    }*/
}

/*************************************Purchase History*************************************/
PurchaseHistory::PurchaseHistory() : TransactionHistory()
{
}

double PurchaseHistory::sell_item_first_in(int qty)
{
    double to_ret = 0.0;
    while (qty > 0)
    {
        PurchaseEntry *earliest = (PurchaseEntry*) this->entries.front();
        int to_subtract = std::min(qty, earliest->get_available_qty());
        qty -= to_subtract;
        earliest->set_available_qty(earliest->get_available_qty() - to_subtract);
        to_ret += to_subtract * (earliest->get_price());
        earliest->update_to_db();
        if (!earliest->get_available_qty())
        {
            this->entries.pop_front();
        }
    }
    return to_ret;
}

double PurchaseHistory::sell_item_last_in(int qty)
{
    double to_ret = 0.0;
    while (qty > 0)
    {
        PurchaseEntry *earliest = (PurchaseEntry*) this->entries.back();
        int to_subtract = std::min(qty, earliest->get_qty());
        qty -= to_subtract;
        earliest->set_available_qty(earliest->get_qty() - to_subtract);
        to_ret += to_subtract * (earliest->get_price());
        earliest->update_to_db();
        if (!earliest->get_available_qty())
        {
            this->entries.pop_back();
        }
    }
    return to_ret;
}
/*************************************Selling History*************************************/
SellingHistory::SellingHistory() : TransactionHistory()
{
}