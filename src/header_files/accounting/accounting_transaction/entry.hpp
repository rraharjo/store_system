#include <iostream>
#include <string>
#include "../enums/account_titles.hpp"
#include "../enums/assets_t_accounts.hpp"
#include "../enums/liabilities_t_accounts.hpp"
#include "../enums/stockholders_t_accounts.hpp"

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
        enums::AccountTitles account;

    public:
        Entry(bool debit, double amount, enums::AccountTitles account, int tAccount);

        bool isDebit();

        double getAmount();

        int getTAccount();

        std::string getTransactionTitle();

        void setTransactionTitle(std::string title);

        long long getTransactionRef();

        void setTransactionRef(long long &reference);

        enums::AccountTitles getAccountTitle();

        std::string getTAccountName();

        std::string to_string();
    };
};
#endif