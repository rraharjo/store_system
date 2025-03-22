#include "util/factory/transaction_factory.hpp"
using namespace util::factory;

AccountingTransactionFactory::AccountingTransactionFactory(util::Date *transaction_date, std::string transaction_name, std::string foreign_id)
{
    this->transaction_date = transaction_date;
    this->transaction_name = transaction_name;
    this->foreign_id = foreign_id;
}

// Purchasing Goods
accounting::Transaction *GoodsPurchaseFactory::create_transaction()
{
    if (this->purchase_amount != this->paid_cash + this->paid_credit)
    {
        throw std::invalid_argument("total paid amount does not match purchase amount");
    }
    accounting::Transaction *new_transaction = new accounting::Transaction(this->transaction_name, this->foreign_id);
    // new_transaction->insert_to_db();
    std::unique_ptr<accounting::Entry> increase_inventory =
        std::make_unique<accounting::Entry>(new_transaction->get_db_code(), true, this->purchase_amount,
                                            util::enums::TAccounts::INVENTORY);
    // increase_inventory->insert_to_db();
    new_transaction->add_entry(std::move(increase_inventory));
    if (this->paid_cash > 0.0)
    {
        std::unique_ptr<accounting::Entry> reduce_cash =
            std::make_unique<accounting::Entry>(new_transaction->get_db_code(), false,
                                                this->paid_cash, util::enums::TAccounts::CASH);
        // reduce_cash->insert_to_db();
        new_transaction->add_entry(std::move(reduce_cash));
    }
    if (this->paid_credit > 0.0)
    {
        std::unique_ptr<accounting::Entry> increase_payable =
            std::make_unique<accounting::Entry>(new_transaction->get_db_code(), false, this->paid_credit,
                                                util::enums::TAccounts::ACCPAYABLE);
        // increase_payable->insert_to_db();
        new_transaction->add_entry(std::move(increase_payable));
    }
    return new_transaction;
};

GoodsPurchaseFactory::GoodsPurchaseFactory(util::Date *transaction_date, std::string transaction_name, std::string foreign_id, double purchase_amount, double paid_cash, double paid_credit) : AccountingTransactionFactory(transaction_date, transaction_name, foreign_id)
{
    this->purchase_amount = purchase_amount;
    this->paid_cash = paid_cash;
    this->paid_credit = paid_credit;
}

// Selling Goods
accounting::Transaction *GoodsSellingFactory::create_transaction()
{
    if (this->sell_amount != this->paid_cash + this->paid_credit)
    {
        throw std::invalid_argument("total paid amount does not match purchase amount");
    }
    accounting::Transaction *new_transaction = new accounting::Transaction(this->transaction_name, this->foreign_id);
    // new_transaction->insert_to_db();
    std::unique_ptr<accounting::Entry> increase_revenue =
        std::make_unique<accounting::Entry>(new_transaction->get_db_code(), false, this->sell_amount,
                              util::enums::TAccounts::REV);
    // increase_revenue->insert_to_db();
    new_transaction->add_entry(std::move(increase_revenue));
    if (this->paid_cash > 0.0)
    {
        std::unique_ptr<accounting::Entry> increase_cash =
            std::make_unique<accounting::Entry>(new_transaction->get_db_code(), true, this->paid_cash,
                                  util::enums::TAccounts::CASH);
        // increase_cash->insert_to_db();
        new_transaction->add_entry(std::move(increase_cash));
    }
    if (this->paid_credit > 0.0)
    {
        accounting::Entry *increase_receivable =
            new accounting::Entry(new_transaction->get_db_code(), true, this->paid_credit,
                                  util::enums::TAccounts::ACCTRCV);
        // increase_receivable->insert_to_db();
        std::unique_ptr<accounting::Entry> to_add(increase_receivable);
        new_transaction->add_entry(std::move(to_add));
    }
    return new_transaction;
}

GoodsSellingFactory::GoodsSellingFactory(util::Date *transaction_date, std::string transaction_name, std::string foreign_id, double sell_amount, double paid_cash, double paid_credit) : AccountingTransactionFactory(transaction_date, transaction_name, foreign_id)
{
    this->sell_amount = sell_amount;
    this->paid_cash = paid_cash;
    this->paid_credit = paid_credit;
}

// Adjusting Cost of Goods Sold
accounting::Transaction *GoodsSoldCOGSFactory::create_transaction()
{
    accounting::Transaction *new_transaction = new accounting::Transaction(this->transaction_name, this->foreign_id);
    // new_transaction->insert_to_db();
    std::unique_ptr<accounting::Entry> reduce_inventory =
        std::make_unique<accounting::Entry>(new_transaction->get_db_code(), false, this->cogs,
                                            util::enums::TAccounts::INVENTORY);
    // reduce_inventory->insert_to_db();
    new_transaction->add_entry(std::move(reduce_inventory));
    std::unique_ptr<accounting::Entry> increase_cogs =
        std::make_unique<accounting::Entry>(new_transaction->get_db_code(), true, this->cogs,
                                            util::enums::TAccounts::COGS);
    // increase_cogs->insert_to_db();
    new_transaction->add_entry(std::move(increase_cogs));
    return new_transaction;
}

GoodsSoldCOGSFactory::GoodsSoldCOGSFactory(util::Date *transaction_date, std::string transaction_name,
                                           std::string foreign_id, double cogs)
    : AccountingTransactionFactory(transaction_date, transaction_name, foreign_id)
{
    this->cogs = cogs;
}

// Buy Equipment Transaction
accounting::Transaction *BuyEquipmentFactory::create_transaction()
{
    if (this->equipment_value != this->paid_cash + this->paid_credit)
    {
        throw std::invalid_argument("total paid amount does not match purchase amount");
    }
    accounting::Transaction *new_transaction =
        new accounting::Transaction(this->transaction_name, this->foreign_id);
    // new_transaction->insert_to_db();
    std::unique_ptr<accounting::Entry> increase_asset =
        std::make_unique<accounting::Entry>(new_transaction->get_db_code(), true, this->equipment_value,
                                            util::enums::TAccounts::EQUIPMENT);
    // increase_asset->insert_to_db();
    new_transaction->add_entry(std::move(increase_asset));
    if (this->paid_cash > 0.0)
    {
        std::unique_ptr<accounting::Entry> reduce_cash =
            std::make_unique<accounting::Entry>(new_transaction->get_db_code(), false, this->paid_cash,
                                                util::enums::TAccounts::CASH);
        // reduce_cash->insert_to_db();
        new_transaction->add_entry(std::move(reduce_cash));
    }
    if (this->paid_credit)
    {
        std::unique_ptr<accounting::Entry> increase_payable =
            std::make_unique<accounting::Entry>(new_transaction->get_db_code(), false, this->paid_credit,
                                                util::enums::TAccounts::ACCPAYABLE);
        // increase_payable->insert_to_db();
        new_transaction->add_entry(std::move(increase_payable));
    }
    return new_transaction;
}

BuyEquipmentFactory::BuyEquipmentFactory(util::Date *transaction_date, std::string transaction_name,
                                         std::string foreign_id, double equipment_value, double paid_cash, double paid_credit)
    : AccountingTransactionFactory(transaction_date, transaction_name, foreign_id)
{
    this->equipment_value = equipment_value;
    this->paid_cash = paid_cash;
    this->paid_credit = paid_credit;
}

// Sell Equipment Transaction
accounting::Transaction *SellEquipmentFactory::create_transaction()
{
    accounting::Transaction *new_transaction = new accounting::Transaction(this->transaction_name, this->foreign_id);
    // new_transaction->insert_to_db();
    if (this->paid_cash > 0.0)
    {
        std::unique_ptr<accounting::Entry> increase_cash =
            std::make_unique<accounting::Entry>(new_transaction->get_db_code(), true, this->paid_cash,
                                                util::enums::TAccounts::CASH);
        // increase_cash->insert_to_db();
        new_transaction->add_entry(std::move(increase_cash));
    }
    if (this->paid_credit > 0.0)
    {
        std::unique_ptr<accounting::Entry> increase_receivable =
            std::make_unique<accounting::Entry>(new_transaction->get_db_code(), true, this->paid_credit,
                                                util::enums::TAccounts::ACCTRCV);
        // increase_receivable->insert_to_db();
        new_transaction->add_entry(std::move(increase_receivable));
    }
    std::unique_ptr<accounting::Entry> reduce_equipment =
        std::make_unique<accounting::Entry>(new_transaction->get_db_code(), false, this->initial_value,
                                            util::enums::TAccounts::EQUIPMENT);
    // reduce_equipment->insert_to_db();
    new_transaction->add_entry(std::move(reduce_equipment));
    std::unique_ptr<accounting::Entry> reduce_accumulated_depreciation =
        std::make_unique<accounting::Entry>(new_transaction->get_db_code(), true, this->accumulated_depreciation,
                                            util::enums::TAccounts::ACCUMDEPRECIATION);
    // reduce_accumulated_depreciation->insert_to_db();
    new_transaction->add_entry(std::move(reduce_accumulated_depreciation));
    double revenue = this->paid_cash + this->paid_credit + this->accumulated_depreciation - this->initial_value;
    if (revenue > 0)
    { // Gain
        std::unique_ptr<accounting::Entry> increase_revenue =
            std::make_unique<accounting::Entry>(new_transaction->get_db_code(), false, revenue,
                                                util::enums::TAccounts::REV); // Shouldn't it be loss
        // increase_revenue->insert_to_db();
        new_transaction->add_entry(std::move(increase_revenue));
    }

    if (revenue < 0)
    { // Loss
        std::unique_ptr<accounting::Entry> reduce_revenue =
            std::make_unique<accounting::Entry>(new_transaction->get_db_code(), true, std::abs(revenue),
                                                util::enums::TAccounts::REV);
        // reduce_revenue->insert_to_db();
        new_transaction->add_entry(std::move(reduce_revenue));
    }
    return new_transaction;
}

SellEquipmentFactory::SellEquipmentFactory(util::Date *transaction_date, std::string transaction_name, std::string foreign_id, double accumulated_depreciation, double initial_value, double paid_cash, double paid_credit) : AccountingTransactionFactory(transaction_date, transaction_name, foreign_id)
{
    this->accumulated_depreciation = accumulated_depreciation;
    this->initial_value = initial_value;
    this->paid_cash = paid_cash;
    this->paid_credit = paid_credit;
}

// Apply Depreciation Transaction
accounting::Transaction *ApplyDepreciationFactory::create_transaction()
{
    accounting::Transaction *new_transaction = new accounting::Transaction(this->transaction_name, this->foreign_id);
    // new_transaction->insert_to_db();
    std::unique_ptr<accounting::Entry> increase_accumulated_depreciation =
        std::make_unique<accounting::Entry>(new_transaction->get_db_code(), false, this->depreciation_amount,
                                            util::enums::TAccounts::ACCUMDEPRECIATION);
    // increase_accumulated_depreciation->insert_to_db();
    new_transaction->add_entry(std::move(increase_accumulated_depreciation));
    std::unique_ptr<accounting::Entry> reduce_equipment =
        std::make_unique<accounting::Entry>(new_transaction->get_db_code(), true, this->depreciation_amount,
                                            util::enums::TAccounts::DEPREXP);
    // reduce_equipment->insert_to_db();
    new_transaction->add_entry(std::move(reduce_equipment));
    return new_transaction;
}

ApplyDepreciationFactory::ApplyDepreciationFactory(util::Date *transaction_date, std::string transaction_name, std::string foreign_id, double depreciation_amount) : AccountingTransactionFactory(transaction_date, transaction_name, foreign_id)
{
    this->depreciation_amount = depreciation_amount;
}

// Pay Wages Transaction
accounting::Transaction *EmployeeWagesFactory::create_transaction()
{
    accounting::Transaction *new_transaction = new accounting::Transaction(this->transaction_name, this->foreign_id);
    // new_transaction->insert_to_db();
    std::unique_ptr<accounting::Entry> reduce_cash =
        std::make_unique<accounting::Entry>(new_transaction->get_db_code(), false, this->wages_amount,
                                            util::enums::TAccounts::CASH);
    // reduce_cash->insert_to_db();
    new_transaction->add_entry(std::move(reduce_cash));
    std::unique_ptr<accounting::Entry> increase_expense =
        std::make_unique<accounting::Entry>(new_transaction->get_db_code(), true, this->wages_amount,
                                            util::enums::TAccounts::WAGEEXP);
    // increase_expense->insert_to_db();
    new_transaction->add_entry(std::move(increase_expense));
    return new_transaction;
}

EmployeeWagesFactory::EmployeeWagesFactory(util::Date *transaction_date, std::string transaction_name, std::string foreign_id, double wages_amount) : AccountingTransactionFactory(transaction_date, transaction_name, foreign_id)
{
}

// Closing the book
accounting::Transaction *ClosingTemporaryAccountsFactory::create_transaction()
{
    double retained_earnings_credit = 0.0;
    accounting::Transaction *closing_the_book =
        new accounting::Transaction(this->transaction_name);
    // closing_the_book->insert_to_db();
    std::unique_ptr<accounting::Entry> temporary = NULL;
    for (accounting::TAccount *t_account : this->t_accounts)
    {
        double t_account_debit = t_account->get_debit_amount() - t_account->get_credit_amount();
        if (t_account_debit > 0) // Zero it with credit amount, reduce retained earnings
        {
            temporary = std::make_unique<accounting::Entry>(closing_the_book->get_db_code(), false, t_account_debit, t_account->get_title());
            // temporary->insert_to_db();
            closing_the_book->add_entry(std::move(temporary));
            retained_earnings_credit -= t_account_debit;
        }
        if (t_account_debit < 0) // Zero it with debit amount, increase retained earnings
        {
            temporary = std::make_unique<accounting::Entry>(closing_the_book->get_db_code(), true, std::abs(t_account_debit), t_account->get_title());
            // temporary->insert_to_db();
            closing_the_book->add_entry(std::move(temporary));
            retained_earnings_credit += std::abs(t_account_debit);
        }
    }
    bool debit_entry;
    if (retained_earnings_credit > 0)
    {
        debit_entry = false;
    }
    else
    {
        debit_entry = true;
    }
    temporary = std::make_unique<accounting::Entry>(closing_the_book->get_db_code(),
                                                    debit_entry,
                                                    std::abs(retained_earnings_credit),
                                                    util::enums::TAccounts::RETAINEDEARNINGS);
    // temporary->insert_to_db();
    closing_the_book->add_entry(std::move(temporary));
    return closing_the_book;
}

ClosingTemporaryAccountsFactory::ClosingTemporaryAccountsFactory(
    util::Date *transaction_date,
    std::string transaction_name,
    std::vector<accounting::TAccount *> &temporary_accounts)
    : AccountingTransactionFactory(transaction_date, transaction_name, "")
{
    this->t_accounts = temporary_accounts;
}