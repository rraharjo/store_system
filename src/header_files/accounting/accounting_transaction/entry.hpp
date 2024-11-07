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
        static util::Table *classTable;

        static std::vector<Entry *> generateFromDB(std::string);

        bool debit;
        double amount;
        std::string transactionTitle;
        util::enums::TAccounts tAccount;
        util::enums::AccountTitles account;
        std::string transactionDB;

    protected:
        std::vector<std::string> getInsertParameter() override;

        std::vector<std::string> getUpdateParameter() override;

    public:
        void insertToDB() override;

        void updateToDB() override;

        Entry(std::string transactionDBCode, bool debit, double amount, util::enums::TAccounts tAccount,
              util::enums::AccountTitles account);

        Entry(std::string dbCode, std::string transactionDBCode, bool debit, double amount,
              util::enums::TAccounts tAccount, util::enums::AccountTitles account);

        bool isDebit();

        double getAmount();

        std::string getTransactionDB();

        std::string getTransactionTitle();

        util::enums::TAccounts getTAccount();

        util::enums::AccountTitles getAccountTitle();

        std::string getTAccountName();

        std::string getAccountTitleName();

        void setTransactionDB(std::string transactionDB);

        void setTransactionTitle(std::string title);

        std::string to_string();

        friend class Transaction;
    };
};
#endif