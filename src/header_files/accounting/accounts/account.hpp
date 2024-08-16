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
        std::string name;

    protected:
        Account(bool debit, std::string name);
        
        std::vector<TAccount *> tAccounts;

        void addTAccount(TAccount *tAccount);

    public:
        std::string getName();

        void addEntry(Entry *entry);

        std::string to_string();

        virtual void initiateTAccount() = 0;
    };
};
#endif