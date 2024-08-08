#include <iostream>
#include <vector>
#include "../accounting_transaction/transaction.hpp"

#ifndef TACCOUNT
#define TACCOUNT
namespace accounting
{
    class TAccount
    {
    private:
        std::string title;
        std::vector<Entry *> debitEntries;
        std::vector<Entry *> creditEntries;

    public:
        TAccount(std::string title);
        
        std::string getTitle();

        void addEntry(Entry *entry);

        double getDebitAmount();

        double getCreditAmount();

        double getTotalAmount();

        std::string to_string();
    };
};
#endif