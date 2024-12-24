#include <string>
#include <vector>
#include "util/date.hpp"
#include "util/database/tables.hpp"
#include "util/class/base_class.hpp"
#include "inventory/transaction/entry.hpp"
#ifndef STORETRANSACTION_HPP
#define STORETRANSACTION_HPP
namespace store
{
    class Transaction : public util::baseclass::HasTable
    {
    private:
        util::Date *transaction_date;
        double paid_cash;
        double paid_credit;
        std::vector<inventory::Entry *> entries;

    protected:
        bool is_finished;

        Transaction(util::Date *transaction_date, double paid_cash, double paid_credit);

        Transaction(util::Date *transaction_date);

    public:
        util::Date *get_date();

        double get_paid_cash();

        double get_paid_credit();

        double get_transaction_amount();

        void add_entry(inventory::Entry *entry);

        void set_paid_cash(double amount);

        void set_paid_credit(double amount);

        std::vector<inventory::Entry *> get_all_entries();
    };

    /*****************************************PURCHASETRANSACTION*****************************************/

    class PurchaseTransaction : public Transaction
    {
    private:
        static util::Table *class_table;
        std::string seller;

    protected:
        std::vector<std::string> get_insert_parameter() override;

        std::vector<std::string> get_update_parameter() override;

    public:
        void insert_to_db() override;

        void update_to_db() override;

        PurchaseTransaction(std::string seller, util::Date *purchase_date);

        std::string get_seller();
    };

    /*****************************************SELLINGTRANSACTION*****************************************/

    class SellingTransaction : public Transaction
    {

    private:
        static util::Table *class_table;

    public:
        void insert_to_db() override;

        void update_to_db() override;

        SellingTransaction(util::Date *transaction_date);

    protected:
        std::vector<std::string> get_insert_parameter() override;

        std::vector<std::string> get_update_parameter() override;
    };
};
#endif