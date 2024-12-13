#include <iostream>
#include <algorithm>
#include <vector>
#include "accounting/accounting_transaction/entry.hpp"
#include "util/date.hpp"
#include "util/class/base_class.hpp"
#include "util/database/tables.hpp"

#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP
namespace accounting
{
    class Transaction : public util::baseclass::HasTable
    {
    private:
        static util::Table *class_table;
        std::vector<Entry *> debit_entries;
        std::vector<Entry *> credit_entries;
        std::string name;
        util::Date *transaction_date;
        std::string entity_id;

    protected:
        std::vector<std::string> get_insert_parameter() override;

        std::vector<std::string> get_update_parameter() override;

    public:
        static std::vector<Transaction *> generate_from_database();

        void insert_to_db() override;

        void update_to_db() override;

        Transaction(std::string db_code, std::string name, util::Date *transaction_date, std::string pid);

        Transaction(std::string name, util::Date *transaction_date, std::string pid);

        Transaction(std::string name, util::Date *transaction_date);

        Transaction(std::string name, std::string pid);

        Transaction(std::string name);

        ~Transaction();

        std::vector<Entry *> &get_debit_entries();

        std::vector<Entry *> &get_credit_entries();

        double get_debit_amount();

        double get_credit_amount();

        void add_entry(Entry *entry);

        bool is_balanced();

        std::string to_string();
    };
};
#endif