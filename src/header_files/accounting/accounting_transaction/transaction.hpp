#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
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
        std::vector<std::shared_ptr<Entry>> debit_entries;
        std::vector<std::shared_ptr<Entry>> credit_entries;
        std::string name;
        std::unique_ptr<util::Date> transaction_date;
        std::string entity_id;

    public:
        Transaction(std::string db_code, std::string name, util::Date *transaction_date, std::string pid);

        Transaction(std::string name, util::Date *transaction_date, std::string pid);

        Transaction(std::string name, util::Date *transaction_date);

        Transaction(std::string name, std::string pid);

        Transaction(std::string name);

        ~Transaction();

        std::string get_name();

        std::string get_entity_id();

        util::Date* get_transaction_date();

        std::vector<std::shared_ptr<Entry>> get_debit_entries();

        std::vector<std::shared_ptr<Entry>> get_credit_entries();

        double get_debit_amount();

        double get_credit_amount();

        void add_entry(std::unique_ptr<Entry> entry);

        bool is_balanced();

        std::string to_string();
    };
};
#endif