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
        std::vector<Entry *> debitEntries;
        std::vector<Entry *> creditEntries;
        std::string name;
        util::Date *transactionDate;
        int dbCode;

    protected:
        void setTable() override;

        std::vector<std::string> getInsertParameter() override;

    public:
        Transaction(std::string name, util::Date *transactionDate);

        Transaction(std::string name);

        std::vector<Entry *> &getDebitEntries();

        std::vector<Entry *> &getCreditEntries();

        double getDebitAmount();

        double getCreditAmount();

        void addEntry(Entry *entry);

        bool isBalanced();
    };
};
#endif