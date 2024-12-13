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
        std::map<util::enums::TAccounts, TAccount *> t_accounts;

        Account(bool debit, util::enums::AccountTitles title);

        virtual ~Account();

        void add_t_account(util::enums::TAccounts t_account);

    public:
        util::enums::AccountTitles get_title();

        std::string get_title_name();

        double get_total_debit();

        double get_total_credit();

        void add_entry(Entry *entry);

        std::string to_string();

        virtual void initiate_t_account() = 0;
    };

    class Assets : public Account
    {
    public:
        Assets();

        void initiate_t_account() override;
    };

    class Liabilities : public Account
    {
    public:
        Liabilities();

        void initiate_t_account() override;
    };

    class StockholdersEquityAccount : public Account
    {
    public:
        StockholdersEquityAccount();

        void initiate_t_account() override;

        std::vector<TAccount *> get_temporary_accounts();
    };
};
#endif