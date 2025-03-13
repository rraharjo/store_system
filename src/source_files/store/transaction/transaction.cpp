#include "store/transaction/transaction.hpp"
using namespace store;

Transaction::Transaction(std::string db_code, util::Date *transaction_date, double paid_cash, double paid_credit)
    : util::baseclass::HasTable()
{
    this->set_db_code(db_code);
    this->transaction_date = transaction_date;
    this->paid_cash = paid_cash;
    this->paid_credit = paid_credit;
    this->is_finished = this->paid_credit == 0.0 ? true : false;
    this->entries = {};
}

Transaction::Transaction(util::Date *transaction_date)
    : Transaction::Transaction("", transaction_date, 0, 0)
{
}

double Transaction::get_paid_cash()
{
    return this->paid_cash;
}

double Transaction::get_paid_credit()
{
    return this->paid_credit;
}

double Transaction::get_transaction_amount()
{
    double to_ret = 0.0;
    for (inventory::Entry *entry : this->entries)
    {
        to_ret += entry->get_price() * entry->get_qty();
    }
    return to_ret;
}

util::Date *Transaction::get_date()
{
    return this->transaction_date;
}

void Transaction::add_entry(inventory::Entry *entry)
{
    this->entries.push_back(entry);
}

void Transaction::set_paid_cash(double amount)
{
    this->paid_cash = amount;
    this->is_finished = this->paid_cash == this->get_transaction_amount() ? true : false;
}

void Transaction::set_paid_credit(double amount)
{
    this->paid_credit = amount;
    this->is_finished = this->paid_credit == 0 ? true : false;
}

std::vector<inventory::Entry *> Transaction::get_all_entries()
{
    return this->entries;
}

/************************PURCHASETRANSACTION****************************/


PurchaseTransaction::PurchaseTransaction(std::string seller, util::Date *purchase_date) : Transaction::Transaction(purchase_date)
{
    this->seller = seller;
}

PurchaseTransaction::PurchaseTransaction(std::string db_code,
                                         std::string seller,
                                         util::Date *purchase_date,
                                         double paid_cash,
                                         double paid_credit)
    : Transaction::Transaction(db_code, purchase_date, paid_cash, paid_credit)
{
    this->seller = seller;
}

std::string PurchaseTransaction::get_seller()
{
    return this->seller;
}

/*********************SELLINGTRANSACTION***********************/

SellingTransaction::SellingTransaction(util::Date *transaction_date) : Transaction::Transaction(transaction_date)
{
}

SellingTransaction::SellingTransaction(std::string db_code,
                                       util::Date *transaction_date,
                                       double paid_cash,
                                       double paid_credit)
    : Transaction::Transaction(db_code, transaction_date, paid_cash, paid_credit)
{
}