#include "util/factory/transaction_factory.hpp"
using namespace util::factory;

AccountingTransactionFactory::AccountingTransactionFactory(std::unique_ptr<util::Date> transaction_date, std::string transaction_name, std::string foreign_id)
{
    this->transaction_date = std::move(transaction_date);
    this->transaction_name = transaction_name;
    this->foreign_id = foreign_id;
}

// Purchasing Goods
std::unique_ptr<accounting::Transaction> GoodsPurchaseFactory::create_transaction()
{
    if (this->purchase_amount != this->paid_cash + this->paid_credit)
    {
        throw std::invalid_argument("total paid amount does not match purchase amount");
    }
    std::unique_ptr<accounting::Transaction> new_transaction =
        std::make_unique<accounting::Transaction>(this->transaction_name, this->foreign_id);
    std::unique_ptr<accounting::Entry> increase_inventory =
        std::make_unique<accounting::Entry>(new_transaction->get_db_code(), true, this->purchase_amount,
                                            util::enums::TAccounts::INVENTORY);
    new_transaction->add_entry(std::move(increase_inventory));
    if (this->paid_cash > 0.0)
    {
        std::unique_ptr<accounting::Entry> reduce_cash =
            std::make_unique<accounting::Entry>(new_transaction->get_db_code(), false,
                                                this->paid_cash, util::enums::TAccounts::CASH);
        new_transaction->add_entry(std::move(reduce_cash));
    }
    if (this->paid_credit > 0.0)
    {
        std::unique_ptr<accounting::Entry> increase_payable =
            std::make_unique<accounting::Entry>(new_transaction->get_db_code(), false, this->paid_credit,
                                                util::enums::TAccounts::ACCPAYABLE);
        new_transaction->add_entry(std::move(increase_payable));
    }
    return std::move(new_transaction);
};

GoodsPurchaseFactory::GoodsPurchaseFactory(std::unique_ptr<util::Date> transaction_date,
                                           std::string transaction_name,
                                           std::string foreign_id,
                                           double purchase_amount,
                                           double paid_cash,
                                           double paid_credit)
    : AccountingTransactionFactory(std::move(transaction_date), transaction_name, foreign_id)
{
    this->purchase_amount = purchase_amount;
    this->paid_cash = paid_cash;
    this->paid_credit = paid_credit;
}

// Selling Goods
std::unique_ptr<accounting::Transaction> GoodsSellingFactory::create_transaction()
{
    if (this->sell_amount != this->paid_cash + this->paid_credit)
    {
        throw std::invalid_argument("total paid amount does not match purchase amount");
    }
    std::unique_ptr<accounting::Transaction> new_transaction =
        std::make_unique<accounting::Transaction>(this->transaction_name, this->foreign_id);
    std::unique_ptr<accounting::Entry> increase_revenue =
        std::make_unique<accounting::Entry>(new_transaction->get_db_code(), false, this->sell_amount,
                                            util::enums::TAccounts::REV);
    new_transaction->add_entry(std::move(increase_revenue));
    if (this->paid_cash > 0.0)
    {
        std::unique_ptr<accounting::Entry> increase_cash =
            std::make_unique<accounting::Entry>(new_transaction->get_db_code(), true, this->paid_cash,
                                                util::enums::TAccounts::CASH);
        new_transaction->add_entry(std::move(increase_cash));
    }
    if (this->paid_credit > 0.0)
    {
        accounting::Entry *increase_receivable =
            new accounting::Entry(new_transaction->get_db_code(), true, this->paid_credit,
                                  util::enums::TAccounts::ACCTRCV);
        std::unique_ptr<accounting::Entry> to_add(increase_receivable);
        new_transaction->add_entry(std::move(to_add));
    }
    return std::move(new_transaction);
}

GoodsSellingFactory::GoodsSellingFactory(std::unique_ptr<util::Date> transaction_date,
                                         std::string transaction_name,
                                         std::string foreign_id,
                                         double sell_amount,
                                         double paid_cash,
                                         double paid_credit)
    : AccountingTransactionFactory(std::move(transaction_date), transaction_name, foreign_id)
{
    this->sell_amount = sell_amount;
    this->paid_cash = paid_cash;
    this->paid_credit = paid_credit;
}

// Adjusting Cost of Goods Sold
std::unique_ptr<accounting::Transaction> GoodsSoldCOGSFactory::create_transaction()
{
    std::unique_ptr<accounting::Transaction> new_transaction =
        std::make_unique<accounting::Transaction>(this->transaction_name, this->foreign_id);
    std::unique_ptr<accounting::Entry> reduce_inventory =
        std::make_unique<accounting::Entry>(new_transaction->get_db_code(), false, this->cogs,
                                            util::enums::TAccounts::INVENTORY);
    new_transaction->add_entry(std::move(reduce_inventory));
    std::unique_ptr<accounting::Entry> increase_cogs =
        std::make_unique<accounting::Entry>(new_transaction->get_db_code(), true, this->cogs,
                                            util::enums::TAccounts::COGS);
    new_transaction->add_entry(std::move(increase_cogs));
    return std::move(new_transaction);
}

GoodsSoldCOGSFactory::GoodsSoldCOGSFactory(std::unique_ptr<util::Date> transaction_date, std::string transaction_name,
                                           std::string foreign_id, double cogs)
    : AccountingTransactionFactory(std::move(transaction_date), transaction_name, foreign_id)
{
    this->cogs = cogs;
}

// Buy Equipment Transaction
std::unique_ptr<accounting::Transaction> BuyEquipmentFactory::create_transaction()
{
    if (this->equipment_value != this->paid_cash + this->paid_credit)
    {
        throw std::invalid_argument("total paid amount does not match purchase amount");
    }
    std::unique_ptr<accounting::Transaction> new_transaction =
        std::make_unique<accounting::Transaction>(this->transaction_name, this->foreign_id);
    std::unique_ptr<accounting::Entry> increase_asset =
        std::make_unique<accounting::Entry>(new_transaction->get_db_code(), true, this->equipment_value,
                                            util::enums::TAccounts::EQUIPMENT);
    new_transaction->add_entry(std::move(increase_asset));
    if (this->paid_cash > 0.0)
    {
        std::unique_ptr<accounting::Entry> reduce_cash =
            std::make_unique<accounting::Entry>(new_transaction->get_db_code(), false, this->paid_cash,
                                                util::enums::TAccounts::CASH);
        new_transaction->add_entry(std::move(reduce_cash));
    }
    if (this->paid_credit)
    {
        std::unique_ptr<accounting::Entry> increase_payable =
            std::make_unique<accounting::Entry>(new_transaction->get_db_code(), false, this->paid_credit,
                                                util::enums::TAccounts::ACCPAYABLE);
        new_transaction->add_entry(std::move(increase_payable));
    }
    return new_transaction;
}

BuyEquipmentFactory::BuyEquipmentFactory(std::unique_ptr<util::Date> transaction_date, std::string transaction_name,
                                         std::string foreign_id, double equipment_value, double paid_cash, double paid_credit)
    : AccountingTransactionFactory(std::move(transaction_date), transaction_name, foreign_id)
{
    this->equipment_value = equipment_value;
    this->paid_cash = paid_cash;
    this->paid_credit = paid_credit;
}

// Sell Equipment Transaction
std::unique_ptr<accounting::Transaction> SellEquipmentFactory::create_transaction()
{
    std::unique_ptr<accounting::Transaction> new_transaction = std::make_unique<accounting::Transaction>(this->transaction_name, this->foreign_id);
    if (this->paid_cash > 0.0)
    {
        std::unique_ptr<accounting::Entry> increase_cash =
            std::make_unique<accounting::Entry>(new_transaction->get_db_code(), true, this->paid_cash,
                                                util::enums::TAccounts::CASH);
        new_transaction->add_entry(std::move(increase_cash));
    }
    if (this->paid_credit > 0.0)
    {
        std::unique_ptr<accounting::Entry> increase_receivable =
            std::make_unique<accounting::Entry>(new_transaction->get_db_code(), true, this->paid_credit,
                                                util::enums::TAccounts::ACCTRCV);
        new_transaction->add_entry(std::move(increase_receivable));
    }
    std::unique_ptr<accounting::Entry> reduce_equipment =
        std::make_unique<accounting::Entry>(new_transaction->get_db_code(), false, this->initial_value,
                                            util::enums::TAccounts::EQUIPMENT);
    new_transaction->add_entry(std::move(reduce_equipment));
    std::unique_ptr<accounting::Entry> reduce_accumulated_depreciation =
        std::make_unique<accounting::Entry>(new_transaction->get_db_code(), true, this->accumulated_depreciation,
                                            util::enums::TAccounts::ACCUMDEPRECIATION);
    new_transaction->add_entry(std::move(reduce_accumulated_depreciation));
    double revenue = this->paid_cash + this->paid_credit + this->accumulated_depreciation - this->initial_value;
    if (revenue > 0)
    { // Gain
        std::unique_ptr<accounting::Entry> increase_revenue =
            std::make_unique<accounting::Entry>(new_transaction->get_db_code(), false, revenue,
                                                util::enums::TAccounts::REV); // Shouldn't it be loss
        new_transaction->add_entry(std::move(increase_revenue));
    }

    if (revenue < 0)
    { // Loss
        std::unique_ptr<accounting::Entry> reduce_revenue =
            std::make_unique<accounting::Entry>(new_transaction->get_db_code(), true, std::abs(revenue),
                                                util::enums::TAccounts::REV);
        new_transaction->add_entry(std::move(reduce_revenue));
    }
    return std::move(new_transaction);
}

SellEquipmentFactory::SellEquipmentFactory(std::unique_ptr<util::Date> transaction_date,
                                           std::string transaction_name,
                                           std::string foreign_id,
                                           double accumulated_depreciation,
                                           double initial_value,
                                           double paid_cash,
                                           double paid_credit)
    : AccountingTransactionFactory(std::move(transaction_date), transaction_name, foreign_id)
{
    this->accumulated_depreciation = accumulated_depreciation;
    this->initial_value = initial_value;
    this->paid_cash = paid_cash;
    this->paid_credit = paid_credit;
}

// Apply Depreciation Transaction
std::unique_ptr<accounting::Transaction> ApplyDepreciationFactory::create_transaction()
{
    std::unique_ptr<accounting::Transaction> new_transaction =
        std::make_unique<accounting::Transaction>(this->transaction_name, this->foreign_id);
    std::unique_ptr<accounting::Entry> increase_accumulated_depreciation =
        std::make_unique<accounting::Entry>(new_transaction->get_db_code(), false, this->depreciation_amount,
                                            util::enums::TAccounts::ACCUMDEPRECIATION);
    new_transaction->add_entry(std::move(increase_accumulated_depreciation));
    std::unique_ptr<accounting::Entry> reduce_equipment =
        std::make_unique<accounting::Entry>(new_transaction->get_db_code(), true, this->depreciation_amount,
                                            util::enums::TAccounts::DEPREXP);
    new_transaction->add_entry(std::move(reduce_equipment));
    return std::move(new_transaction);
}

ApplyDepreciationFactory::ApplyDepreciationFactory(std::unique_ptr<util::Date> transaction_date,
                                                   std::string transaction_name,
                                                   std::string foreign_id,
                                                   double depreciation_amount)
    : AccountingTransactionFactory(std::move(transaction_date), transaction_name, foreign_id)
{
    this->depreciation_amount = depreciation_amount;
}

// Pay Wages Transaction
std::unique_ptr<accounting::Transaction> EmployeeWagesFactory::create_transaction()
{
    std::unique_ptr<accounting::Transaction> new_transaction =
        std::make_unique<accounting::Transaction>(this->transaction_name, this->foreign_id);
    std::unique_ptr<accounting::Entry> reduce_cash =
        std::make_unique<accounting::Entry>(new_transaction->get_db_code(), false, this->wages_amount,
                                            util::enums::TAccounts::CASH);
    new_transaction->add_entry(std::move(reduce_cash));
    std::unique_ptr<accounting::Entry> increase_expense =
        std::make_unique<accounting::Entry>(new_transaction->get_db_code(), true, this->wages_amount,
                                            util::enums::TAccounts::WAGEEXP);
    new_transaction->add_entry(std::move(increase_expense));
    return std::move(new_transaction);
}

EmployeeWagesFactory::EmployeeWagesFactory(std::unique_ptr<util::Date> transaction_date,
                                           std::string transaction_name,
                                           std::string foreign_id,
                                           double wages_amount)
    : AccountingTransactionFactory(std::move(transaction_date), transaction_name, foreign_id)
{
}

// Closing the book
std::unique_ptr<accounting::Transaction> ClosingTemporaryAccountsFactory::create_transaction()
{
    double retained_earnings_credit = 0.0;
    std::unique_ptr<accounting::Transaction> closing_the_book =
        std::make_unique<accounting::Transaction>(this->transaction_name);
    std::unique_ptr<accounting::Entry> temporary = NULL;
    for (accounting::TAccount *t_account : this->t_accounts)
    {
        double t_account_debit = t_account->get_debit_amount() - t_account->get_credit_amount();
        if (t_account_debit > 0) // Zero it with credit amount, reduce retained earnings
        {
            temporary = std::make_unique<accounting::Entry>(closing_the_book->get_db_code(), false, t_account_debit, t_account->get_title());
            closing_the_book->add_entry(std::move(temporary));
            retained_earnings_credit -= t_account_debit;
        }
        if (t_account_debit < 0) // Zero it with debit amount, increase retained earnings
        {
            temporary = std::make_unique<accounting::Entry>(
                closing_the_book->get_db_code(),
                true,
                std::abs(t_account_debit),
                t_account->get_title());
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
    closing_the_book->add_entry(std::move(temporary));
    return std::move(closing_the_book);
}

ClosingTemporaryAccountsFactory::ClosingTemporaryAccountsFactory(
    std::unique_ptr<util::Date> transaction_date,
    std::string transaction_name,
    std::vector<accounting::TAccount *> &temporary_accounts)
    : AccountingTransactionFactory(std::move(transaction_date), transaction_name, "")
{
    this->t_accounts = temporary_accounts;
}