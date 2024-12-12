#include <iostream>
#include <vector>
#include <algorithm>
#include "accounting/accounting_transaction/transaction.hpp"
#include "util/enums/accounting/t_accounts.hpp"
#include "util/class/base_class.hpp"

#ifndef TACCOUNT
#define TACCOUNT
namespace accounting
{
    class TAccount : public util::baseclass::HasTable
    {
    private:
        static util::Table *classTable;

        static void initiateTAccountOnDB();

        static TAccount *generateFromDatabase(util::enums::TAccounts);

        util::enums::TAccounts title;
        double debitAmount;
        double creditAmount;
        std::vector<Entry *> debitEntries;
        std::vector<Entry *> creditEntries;

    protected:
        std::vector<std::string> getInsertParameter() override;

        std::vector<std::string> getUpdateParameter() override;

    public:
        void insertToDB() override;

        void updateToDB() override;

        TAccount(util::enums::TAccounts title, double debit, double credit);

        TAccount(util::enums::TAccounts title);

        ~TAccount();

        util::enums::TAccounts getTitle();

        std::string getTitleName();

        void addEntry(Entry *entry);

        double getDebitAmount();

        double getCreditAmount();

        double getTotalAmount();

        std::string to_string();

        friend class Account;

        friend class AccountingSystem;
    };
};
#endif