#include <iostream>
#include <vector>
#include <map>
#include "../t_accounts/t_account.hpp"

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
        Account(bool debit, util::enums::AccountTitles title);

        virtual ~Account();

        std::map<util::enums::TAccounts, TAccount *> tAccounts;

        void addTAccount(util::enums::TAccounts tAccount);

    public:
        util::enums::AccountTitles getTitle();

        std::string getTitleName();

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
    };
};
#endif