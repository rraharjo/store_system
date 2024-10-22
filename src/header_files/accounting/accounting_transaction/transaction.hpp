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
        static util::Table *classTable;
        std::vector<Entry *> debitEntries;
        std::vector<Entry *> creditEntries;
        std::string name;
        util::Date *transactionDate;
        std::string entityID;
        int dbCode;

    protected:
        std::vector<std::string> getInsertParameter() override;

        std::vector<std::string> getUpdateParameter() override;

    public:
        void insertToDB() override;

        void updateToDB() override;
        
        Transaction(std::string name, util::Date *transactionDate);

        Transaction(std::string name);

        Transaction(std::string name, util::Date *transactionDate, std::string pid);

        Transaction(std::string name, std::string pid);

        ~Transaction();

        std::vector<Entry *> &getDebitEntries();

        std::vector<Entry *> &getCreditEntries();

        double getDebitAmount();

        double getCreditAmount();

        void addEntry(Entry *entry);

        bool isBalanced();
    };
};
#endif