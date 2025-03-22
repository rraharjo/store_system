#include "accounting/accounting_transaction/transaction.hpp"
#include "accounting/accounts/t_account.hpp"
#ifndef ACCOUNTINGTRANSACTION_HPP
#define ACCOUNTINGTRANSACTION_HPP
namespace util
{
    namespace factory
    {
        class AccountingTransactionFactory
        {
        protected:
            std::unique_ptr<util::Date> transaction_date;
            std::string transaction_name;
            std::string foreign_id;

            virtual std::unique_ptr<accounting::Transaction> create_transaction() = 0;

            AccountingTransactionFactory(std::unique_ptr<util::Date>, std::string, std::string);
        };

        class GoodsPurchaseFactory : public AccountingTransactionFactory
        {
        private:
            double purchase_amount;
            double paid_cash;
            double paid_credit;

        public:
            std::unique_ptr<accounting::Transaction> create_transaction() override;

            GoodsPurchaseFactory(std::unique_ptr<util::Date>, std::string, std::string,
                                 double purchase_amount, double paid_cash, double paid_credit);
        };

        class GoodsSellingFactory : public AccountingTransactionFactory
        {
        private:
            double sell_amount;
            double paid_cash;
            double paid_credit;

        public:
            std::unique_ptr<accounting::Transaction> create_transaction() override;

            // GoodsSellingFactory &setForeignID(std::string);

            GoodsSellingFactory(std::unique_ptr<util::Date>, std::string, std::string, double sell_amount, double paid_cash, double paid_credit);
        };

        class GoodsSoldCOGSFactory : public AccountingTransactionFactory
        {
        private:
            double cogs;

        public:
            std::unique_ptr<accounting::Transaction> create_transaction() override;

            GoodsSoldCOGSFactory(std::unique_ptr<util::Date>, std::string, std::string, double cogs);
        };

        class BuyEquipmentFactory : public AccountingTransactionFactory
        {
        private:
            double equipment_value;
            double paid_cash;
            double paid_credit;

        public:
            std::unique_ptr<accounting::Transaction> create_transaction() override;

            BuyEquipmentFactory(std::unique_ptr<util::Date>, std::string, std::string,
                                double equipment_value, double paid_cash, double paid_credit);
        };

        class SellEquipmentFactory : public AccountingTransactionFactory
        {
        private:
            double accumulated_depreciation;
            double initial_value;
            double paid_cash;
            double paid_credit;

        public:
            std::unique_ptr<accounting::Transaction> create_transaction() override;

            SellEquipmentFactory(std::unique_ptr<util::Date>, std::string, std::string,
                                 double accumulated_depreciation, double initial_value, double paid_cash, double paid_credit);
        };

        class ApplyDepreciationFactory : public AccountingTransactionFactory
        {
        private:
            double depreciation_amount;

        public:
            std::unique_ptr<accounting::Transaction> create_transaction() override;

            ApplyDepreciationFactory(std::unique_ptr<util::Date>, std::string, std::string, double depreciation_amount);
        };

        class EmployeeWagesFactory : public AccountingTransactionFactory
        {
        private:
            double wages_amount;

        public:
            std::unique_ptr<accounting::Transaction> create_transaction() override;

            EmployeeWagesFactory(std::unique_ptr<util::Date>, std::string, std::string, double wages_amount);
        };

        class ClosingTemporaryAccountsFactory : public AccountingTransactionFactory
        {
        private:
            std::vector<accounting::TAccount *> t_accounts;

        public:
            std::unique_ptr<accounting::Transaction> create_transaction() override;

            ClosingTemporaryAccountsFactory(std::unique_ptr<util::Date>, std::string, std::vector<accounting::TAccount *> &);
        };
    }
}
#endif