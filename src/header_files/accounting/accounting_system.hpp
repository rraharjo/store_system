#include "accounting/accounts/account.hpp"
#include "accounting/accounting_transaction/transaction.hpp"
#include "util/factory/transaction_factory.hpp"

#ifndef ACCOUNTINGSYSTEM_HPP
#define ACCOUNTINGSYSTEM_HPP
namespace accounting
{
    class AccountingSystem
    {
    private:
        static AccountingSystem *instance;

        std::map<std::string, Transaction *> transactions;

        AccountingSystem();

        void addEntry(Entry *entry);

        void addExistingTransaction(Transaction *transaction);

    public:
        Assets *assets;
        Liabilities *liabilities;
        StockholdersEquityAccount *stockholdersEquity;
        static AccountingSystem *getInstance();

        ~AccountingSystem();

        void addTransaction(Transaction *transaction);

        void endYearAdjustment();

        std::string to_string();
    };
};
#endif