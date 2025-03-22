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

TransactionHistory::~TransactionHistory()
{
#ifdef DEBUG
    std::cout << "Deleting Transaction history (parent)" << std::endl;
#endif
}

void TransactionHistory::add_entry(std::shared_ptr<Entry> entry)
{
    // std::shared_ptr<Entry> to_add(entry);
    this->entries.push_back(entry);
}

std::vector<std::shared_ptr<Entry>> TransactionHistory::get_entries()
{
    int entries_size = this->entries.size();
    std::vector<std::shared_ptr<Entry>> to_ret;
    while (entries_size)
    {
        std::shared_ptr<Entry> temp_entry = this->entries.front();
        this->entries.pop_front();
        to_ret.push_back(temp_entry);
        this->entries.push_back(temp_entry);
        entries_size--;
    }
    return to_ret;
}

/*************************************Purchase History*************************************/
PurchaseHistory::PurchaseHistory() : TransactionHistory()
{
}

PurchaseHistory::~PurchaseHistory()
{
#ifdef DEBUG
    std::cout << "Deleting Purchase history" << std::endl;
#endif
}

double PurchaseHistory::sell_item_first_in(int qty)
{
    double to_ret = 0.0;
    while (qty > 0)
    {
        PurchaseEntry *earliest = (PurchaseEntry *)this->entries.front().get();
        int to_subtract = std::min(qty, earliest->get_available_qty());
        qty -= to_subtract;
        earliest->set_available_qty(earliest->get_available_qty() - to_subtract);
        to_ret += to_subtract * (earliest->get_price());
        // earliest->update_to_db();
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
        PurchaseEntry *earliest = (PurchaseEntry *)this->entries.back().get();
        int to_subtract = std::min(qty, earliest->get_qty());
        qty -= to_subtract;
        earliest->set_available_qty(earliest->get_qty() - to_subtract);
        to_ret += to_subtract * (earliest->get_price());
        // earliest->update_to_db();
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

SellingHistory::~SellingHistory()
{
#ifdef DEBUG
    std::cout << "Deleting Selling History" << std::endl;
#endif
}