#include "store/transaction/transaction.hpp"
using namespace store;

Transaction::Transaction(util::enums::PrimaryKeyPrefix primary_key_prefix,
                         std::string db_code,
                         util::Date *transaction_date,
                         double paid_cash,
                         double paid_credit)
    : util::baseclass::HasTable(primary_key_prefix)
{
    this->set_db_code(db_code);
    this->transaction_date.reset(transaction_date);
    this->paid_cash = paid_cash;
    this->paid_credit = paid_credit;
    this->is_finished = this->paid_credit == 0.0 ? true : false;
    this->entries = {};
}

Transaction::Transaction(util::enums::PrimaryKeyPrefix primary_key_prefix, util::Date *transaction_date)
    : Transaction::Transaction(primary_key_prefix, "", transaction_date, 0, 0)
{
}

Transaction::~Transaction()
{
#ifdef DEBUG
    std::cout << "Deleting store transaction" << std::endl;
#endif
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
    for (std::shared_ptr<inventory::Entry> entry : this->entries)
    {
        to_ret += entry.get()->get_price() * entry.get()->get_qty();
    }
    return to_ret;
}

util::Date *Transaction::get_date()
{
    return this->transaction_date.get();
}

void Transaction::add_entry(std::unique_ptr<inventory::Entry> entry)
{
    std::shared_ptr<inventory::Entry> to_add;
    to_add.reset(entry.release());
    this->entries.push_back(to_add);
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

std::vector<std::shared_ptr<inventory::Entry>> Transaction::get_all_entries()
{
    return this->entries;
}

/************************PURCHASETRANSACTION****************************/

PurchaseTransaction::PurchaseTransaction(std::string seller, util::Date *purchase_date)
    : Transaction::Transaction(util::enums::PrimaryKeyPrefix::PURCHASETRANSACTION, purchase_date)
{
    this->seller = seller;
}

PurchaseTransaction::PurchaseTransaction(std::string db_code,
                                         std::string seller,
                                         util::Date *purchase_date,
                                         double paid_cash,
                                         double paid_credit)
    : Transaction::Transaction(util::enums::PrimaryKeyPrefix::PURCHASETRANSACTION, db_code, purchase_date, paid_cash, paid_credit)
{
    this->seller = seller;
}

PurchaseTransaction::~PurchaseTransaction()
{
#ifdef DEBUG
    std::cout << "Deleting Purchase transaction" << std::endl;
#endif
}

std::string PurchaseTransaction::get_seller()
{
    return this->seller;
}

/*********************SELLINGTRANSACTION***********************/

SellingTransaction::SellingTransaction(util::Date *transaction_date)
    : Transaction::Transaction(util::enums::PrimaryKeyPrefix::SELLINGTRANSACTION, transaction_date)
{
}

SellingTransaction::SellingTransaction(std::string db_code,
                                       util::Date *transaction_date,
                                       double paid_cash,
                                       double paid_credit)
    : Transaction::Transaction(util::enums::PrimaryKeyPrefix::SELLINGTRANSACTION, db_code, transaction_date, paid_cash, paid_credit)
{
}

SellingTransaction::~SellingTransaction()
{
#ifdef DEBUG
    std::cout << "Deleting Selling Transaction" << std::endl;
#endif
}