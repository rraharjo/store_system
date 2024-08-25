#pragma once
#include "accounting/accounts/account.hpp"
#include "accounting/accounting_transaction/transaction.hpp"

#ifndef ACCOUNTINGSYSTEM_HPP
#define ACCOUNTINGSYSTEM_HPP
namespace accounting
{
    class AccountingSystem
    {
    private:
        Assets *assets;
        Liabilities *liabilities;
        StockholdersEquityAccount *stockholdersEquity;
        static AccountingSystem *instance;

        AccountingSystem();

        void addEntry(Entry *entry);

    public:
        static AccountingSystem *getInstance();

        ~AccountingSystem();

        void addTransaction(Transaction *transaction);

        std::string to_string();
    };
};
#endif