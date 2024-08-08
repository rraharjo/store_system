#pragma once
#include "./accounts/assets.hpp"
#include "./accounts/liabilities.hpp"
#include "./accounts/stockholders_equity.hpp"
#include "./accounting_transaction/transaction.hpp"

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

        void addTransaction(Transaction *transaction);

        std::string to_string();
    };
};
#endif