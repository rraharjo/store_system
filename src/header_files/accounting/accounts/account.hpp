#include <iostream>
#include <vector>
#include <map>
#include "accounting/accounts/t_account.hpp"

#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP
namespace accounting
{

    class Account
    {
    private:
        bool debit;
        util::enums::AccountTitles title;

    protected:
        std::map<util::enums::TAccounts, TAccount *> tAccounts;

        Account(bool debit, util::enums::AccountTitles title);

        virtual ~Account();

        void addTAccount(util::enums::TAccounts tAccount);

    public:
        util::enums::AccountTitles getTitle();

        std::string getTitleName();

        double getTotalDebit();

        double getTotalCredit();

        void addEntry(Entry *entry);

        std::string to_string();

        virtual void initiateTAccount() = 0;
    };

    class Assets : public Account
    {
    public:
        Assets();

        void initiateTAccount() override;
    };

    class Liabilities : public Account
    {
    public:
        Liabilities();

        void initiateTAccount() override;
    };

    class StockholdersEquityAccount : public Account
    {
    public:
        StockholdersEquityAccount();

        void initiateTAccount() override;

        std::vector<TAccount *> getTemporaryAccounts();
    };
};
#endif