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
        std::string transactionTitle;
        util::enums::TAccounts tAccount;
        util::enums::AccountTitles account;
        int transactionDB;

    protected:
        void setTable() override;

        std::vector<std::string> getInsertParameter() override;

    public:
        Entry(bool debit, double amount, util::enums::AccountTitles account, util::enums::TAccounts tAccount);

        bool isDebit();

        double getAmount();

        int getTransactionDB();

        std::string getTransactionTitle();

        util::enums::TAccounts getTAccount();

        util::enums::AccountTitles getAccountTitle();

        std::string getTAccountName();

        std::string getAccountTitleName();

        void setTransactionDB(int transactionDB);

        void setTransactionTitle(std::string title);

        std::string to_string();
    };
};
#endif