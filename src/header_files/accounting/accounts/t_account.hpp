#include <iostream>
#include <vector>
#include <algorithm>
#include "accounting/accounting_transaction/transaction.hpp"
#include "util/enums/accounting/t_accounts.hpp"
#include "util/class/base_class.hpp"

#ifndef TACCOUNT
#define TACCOUNT
namespace accounting
{
    class TAccount : public util::baseclass::HasTable
    {
    private:
        static util::Table *class_table;

        static void initiate_t_account_on_db();

        static TAccount *generate_from_database(util::enums::TAccounts);

        util::enums::TAccounts title;
        double debit_amount;
        double credit_amount;
        std::vector<Entry *> debit_entries;
        std::vector<Entry *> credit_entries;

    protected:
        std::vector<std::string> get_insert_parameter() override;

        std::vector<std::string> get_update_parameter() override;

    public:
        void insert_to_db() override;

        void update_to_db() override;

        TAccount(util::enums::TAccounts title, double debit, double credit);

        TAccount(util::enums::TAccounts title);

        ~TAccount();

        util::enums::TAccounts get_title();

        std::string get_title_name();

        void add_entry(Entry *entry);

        double get_debit_amount();

        double get_credit_amount();

        double get_total_amount();

        std::string to_string();

        friend class Account;

        friend class AccountingSystem;
    };
};
#endif