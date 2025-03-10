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

        void add_entry(Entry *entry);

        void add_existing_transaction(Transaction *transaction);

    public:
        Assets *assets;
        Liabilities *liabilities;
        StockholdersEquityAccount *stockholders_equity;
        static AccountingSystem *get_instance();

        ~AccountingSystem();

        void add_transaction(Transaction *transaction);

        void end_year_adjustment();

        std::string to_string();
    };
};
#endif