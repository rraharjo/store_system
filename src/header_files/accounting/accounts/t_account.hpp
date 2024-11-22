#include <iostream>
#include <vector>
#include "accounting/accounting_transaction/transaction.hpp"
#include "util/enums/accounting/t_accounts.hpp"

#ifndef TACCOUNT
#define TACCOUNT
namespace accounting
{
    class TAccount// TO DO: add amount of each tAccount; create a new table ?; load all when rebooting
    {
    private:
        util::enums::TAccounts title;
        std::vector<Entry *> debitEntries;
        std::vector<Entry *> creditEntries;

    public:
        TAccount(util::enums::TAccounts title);

        ~TAccount();

        util::enums::TAccounts getTitle();
        
        std::string getTitleName();

        void addEntry(Entry *entry);

        double getDebitAmount();

        double getCreditAmount();

        double getTotalAmount();

        std::string to_string();
    };
};
#endif