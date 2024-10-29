#include "accounting/accounts/account.hpp"
#include "accounting/accounting_transaction/transaction.hpp"

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

    public:
        Assets *assets;
        Liabilities *liabilities;
        StockholdersEquityAccount *stockholdersEquity;
        static AccountingSystem *getInstance();

        ~AccountingSystem();

        void addTransaction(Transaction *transaction);

        std::string to_string();
    };
};
#endif