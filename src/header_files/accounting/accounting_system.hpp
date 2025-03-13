#include "accounting/accounts/account.hpp"
#include "accounting/accounting_transaction/transaction.hpp"
#include "util/factory/transaction_factory.hpp"
#include "util/class/accounting_transaction_collection.hpp"
#include "util/class/t_account_collection.hpp"

#ifndef ACCOUNTINGSYSTEM_HPP
#define ACCOUNTINGSYSTEM_HPP
namespace accounting
{
    class AccountingSystem
    {
    private:
        static AccountingSystem *instance;

        std::unique_ptr<util::baseclass::AccountingTransactionCollection> transactions;

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