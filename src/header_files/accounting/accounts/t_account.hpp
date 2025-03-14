#include <iostream>
#include <vector>
#include <algorithm>
#include "accounting/accounting_transaction/transaction.hpp"
#include "util/enums/accounting/t_accounts.hpp"
#include "util/class/base_class.hpp"

#ifndef TACCOUNT
#define TACCOUNT
//TODO: this class may not be needed
namespace accounting
{
    class TAccount : public util::baseclass::HasTable
    {
    private:
        util::enums::TAccounts title;
        util::enums::AccountTitles account_title;
        double debit_amount;
        double credit_amount;

    public:
        TAccount(util::enums::TAccounts title, util::enums::AccountTitles account_title, double debit, double credit);

        TAccount(util::enums::TAccounts title);

        ~TAccount();

        util::enums::TAccounts get_title();

        std::string get_title_name();

        void add_entry(Entry *entry);

        double get_debit_amount();

        double get_credit_amount();

        double get_total_amount();

        std::string to_string();

        friend class Account;

        friend class AccountingSystem;
    };
};
#endif