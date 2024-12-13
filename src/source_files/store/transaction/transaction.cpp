#include "store/transaction/transaction.hpp"
using namespace store;

Transaction::Transaction(util::Date *transaction_date, double paid_cash, double paid_credit)
    : util::baseclass::HasTable()
{
    this->transaction_date = transaction_date;
    this->paid_cash = paid_cash;
    this->paid_credit = paid_credit;
    this->is_finished = this->paid_credit == 0.0 ? true : false;
    this->entries = {};
}

Transaction::Transaction(util::Date *transaction_date)
    : Transaction::Transaction(transaction_date, 0, 0)
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
util::Table *PurchaseTransaction::class_table = util::PurchaseTransactionTable::get_instance();

void PurchaseTransaction::insert_to_db()
{
    this->insert_to_db_with_table(PurchaseTransaction::class_table);
    for (inventory::Entry *entry : this->get_all_entries())
    {
        entry->set_transaction_db_code(this->get_db_code());
        entry->insert_to_db();
    }
};

void PurchaseTransaction::update_to_db()
{
    this->update_to_db_with_table(PurchaseTransaction::class_table);
};

PurchaseTransaction::PurchaseTransaction(std::string seller, util::Date *purchase_date) : Transaction::Transaction(purchase_date)
{
    this->seller = seller;
}

std::string PurchaseTransaction::get_seller()
{
    return this->seller;
}

std::vector<std::string> PurchaseTransaction::get_insert_parameter()
{
    std::vector<std::string> args;
    args.push_back(util::enums::primary_key_codes_map[util::enums::PrimaryKeyCodes::PURCHASETRANSACTION]);
    args.push_back(this->get_date()->to_db_format());
    args.push_back(this->get_seller());
    args.push_back(std::to_string(this->get_paid_cash()));
    args.push_back(std::to_string(this->get_paid_credit()));
    args.push_back(this->is_finished ? "true" : "false");
    return args;
}

std::vector<std::string> PurchaseTransaction::get_update_parameter()
{
    std::vector<std::string> args;
    args.push_back(this->get_date()->to_db_format());
    args.push_back(this->get_seller());
    args.push_back(std::to_string(this->get_paid_cash()));
    args.push_back(std::to_string(this->get_paid_credit()));
    args.push_back(this->is_finished ? "true" : "false");
    return args;
}

/*********************SELLINGTRANSACTION***********************/

util::Table *SellingTransaction::class_table = util::SellingTransactionTable::get_instance();

void SellingTransaction::insert_to_db()
{
    this->insert_to_db_with_table(SellingTransaction::class_table);
    for (inventory::Entry *entry : this->get_all_entries())
    {
        entry->set_transaction_db_code(this->get_db_code());
        entry->insert_to_db();
    }
};

void SellingTransaction::update_to_db()
{
    this->update_to_db_with_table(SellingTransaction::class_table);
};

SellingTransaction::SellingTransaction(util::Date *transaction_date) : Transaction::Transaction(transaction_date)
{
}

std::vector<std::string> SellingTransaction::get_insert_parameter()
{
    std::vector<std::string> args;
    args.push_back(util::enums::primary_key_codes_map[util::enums::PrimaryKeyCodes::SELLINGTRANSACTION]);
    args.push_back(this->get_date()->to_db_format());
    args.push_back(std::to_string(this->get_paid_cash()));
    args.push_back(std::to_string(this->get_paid_credit()));
    args.push_back(this->is_finished ? "true" : "false");
    return args;
}

std::vector<std::string> SellingTransaction::get_update_parameter()
{
    std::vector<std::string> args;
    args.push_back(this->get_date()->to_db_format());
    args.push_back(std::to_string(this->get_paid_cash()));
    args.push_back(std::to_string(this->get_paid_credit()));
    args.push_back(this->is_finished ? "true" : "false");
    return args;
}