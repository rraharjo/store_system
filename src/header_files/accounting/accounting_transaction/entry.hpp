#include <iostream>
#include <string>
#include "util/enums/accounting/account_titles.hpp"
#include "util/enums/accounting/assets_t_accounts.hpp"
#include "util/enums/accounting/liabilities_t_accounts.hpp"
#include "util/enums/accounting/stockholders_t_accounts.hpp"

#ifndef ENTRY_HPP
#define ENTRY_HPP
namespace accounting
{
    class Entry
    {
    private:
        bool debit;
        double amount;
        int tAccount;
        long long transactionRef;
        std::string transactionTitle;
        util::enums::AccountTitles account;

    public:
        Entry(bool debit, double amount, util::enums::AccountTitles account, int tAccount);

        bool isDebit();

        double getAmount();

        int getTAccount();

        std::string getTransactionTitle();

        void setTransactionTitle(std::string title);

        long long getTransactionRef();

        void setTransactionRef(long long &reference);

        util::enums::AccountTitles getAccountTitle();

        std::string getTAccountName();

        std::string to_string();
    };
};
#endif