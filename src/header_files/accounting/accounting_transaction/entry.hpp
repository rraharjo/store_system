#include <iostream>
#include <string>
#include "util/enums/accounting/t_accounts.hpp"
#include "util/enums/accounting/account_titles.hpp"
#include "util/class/base_class.hpp"

#ifndef ENTRY_HPP
#define ENTRY_HPP
namespace accounting
{
    class Entry : public util::baseclass::HasTable
    {
    private:
        bool debit;
        double amount;
        std::string description;
        util::enums::TAccounts tAccount;
        util::enums::AccountTitles account;
        int dbCode;
        int transactionDB;

        void setDBCode(int dbCode);

        void setTransactionDB(int transactionDB);

    protected:
        void setTable() override;

    public:
        Entry(bool debit, double amount, std::string description, util::enums::AccountTitles account, util::enums::TAccounts tAccount);

        bool isDebit();

        double getAmount();

        int getTransactionDB();

        int getDBCode();

        std::string getDescription();

        util::enums::TAccounts getTAccount();

        util::enums::AccountTitles getAccountTitle();

        std::string getTAccountName();

        std::string getAccountTitleName();

        std::string to_string();
    };
};
#endif