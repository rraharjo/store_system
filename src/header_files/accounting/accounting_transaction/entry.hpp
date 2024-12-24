#include <iostream>
#include <string>
#include "util/enums/accounting/t_accounts.hpp"
#include "util/enums/accounting/account_titles.hpp"
#include "util/class/base_class.hpp"

#ifndef ENTRY_HPP
#define ENTRY_HPP
namespace accounting
{
    class Entry : public util::baseclass::HasTable
    {

    private:
        static util::Table *class_table;

        static std::vector<Entry *> generate_from_db(std::string);

        bool debit;
        double amount;
        std::string transaction_title;
        util::enums::TAccounts t_account;
        util::enums::AccountTitles account;
        std::string transaction_db;

    protected:
        std::vector<std::string> get_insert_parameter() override;

        std::vector<std::string> get_update_parameter() override;

    public:
        void insert_to_db() override;

        void update_to_db() override;

        Entry(std::string transaction_db_code, bool debit, double amount, util::enums::TAccounts t_account);

        Entry(std::string db_code, std::string transaction_db_code, bool debit, double amount,
              util::enums::TAccounts t_account);

        bool is_debit();

        double get_amount();

        std::string get_transaction_db();

        std::string get_transaction_title();

        util::enums::TAccounts get_t_account();

        util::enums::AccountTitles get_account_title();

        std::string get_t_account_name();

        std::string get_account_title_name();

        void set_transaction_db(std::string transaction_db);

        void set_transaction_title(std::string title);

        std::string to_string();

        friend class Transaction;
    };
};
#endif