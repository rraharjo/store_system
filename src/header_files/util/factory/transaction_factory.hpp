#include "accounting/accounting_transaction/transaction.hpp"
#ifndef ACCOUNTINGTRANSACTION_HPP
#define ACCOUNTINGTRANSACTION_HPP
namespace util
{
    namespace factory
    {
        class AccountingTransactionFactory
        {
        protected:
            util::Date *transactionDate;
            std::string transactionName;
            virtual accounting::Transaction *createTransaction() = 0;

            AccountingTransactionFactory(util::Date *, std::string);

            virtual ~AccountingTransactionFactory();
        };

        class GoodsPurchaseFactory : AccountingTransactionFactory
        {
        private:
            double purchaseAmount;
            double paidCash;
            double paidCredit;

        public:
            accounting::Transaction *createTransaction() override;

            GoodsPurchaseFactory(util::Date *, std::string, double purchaseAmount, double paidCash, double paidCredit);

            GoodsPurchaseFactory(std::string, double purchaseAmount, double paidCash, double paidCredit);
        };

        class GoodsSellingFactory : AccountingTransactionFactory
        {
        private:
            double sellAmount;
            double paidCash;
            double paidCredit;

        public:
            accounting::Transaction *createTransaction() override;

            GoodsSellingFactory(util::Date *, std::string, double sellAmount, double paidCash, double paidCredit);

            GoodsSellingFactory(std::string, double sellAmount, double paidCash, double paidCredit);
        };

        class GoodsSoldCOGSFactory : AccountingTransactionFactory
        {
        private:
            double cogs;

        public:
            accounting::Transaction *createTransaction() override;

            GoodsSoldCOGSFactory(util::Date *, std::string, double cogs);

            GoodsSoldCOGSFactory(std::string, double cogs);
        };

        class BuyEquipmentFactory : AccountingTransactionFactory
        {
        private:
            double equipmentValue;
            double paidCash;
            double paidCredit;

        public:
            accounting::Transaction *createTransaction() override;

            BuyEquipmentFactory(util::Date *, std::string, double equipmentValue, double paidCash, double paidCredit);

            BuyEquipmentFactory(std::string, double equipmentValue, double paidCash, double paidCredit);
        };

        class SellEquipmentFactory : AccountingTransactionFactory
        {
        private:
            double accumulatedDepreciation;
            double initialValue;
            double paidCash;
            double paidCredit;

        public:
            accounting::Transaction *createTransaction() override;

            SellEquipmentFactory(util::Date *, std::string, double accumulatedDepreciation, double initialValue, double paidCash, double paidCredit);

            SellEquipmentFactory(std::string, double accumulatedDepreciation, double initialValue, double paidCash, double paidCredit);
        };

        class ApplyDepreciationFactory : AccountingTransactionFactory
        {
        private:
            double depreciationAmount;

        public:
            accounting::Transaction *createTransaction() override;

            ApplyDepreciationFactory(util::Date *, std::string, double depreciationAmount);

            ApplyDepreciationFactory(std::string, double depreciationAmount);
        };

        class EmployeeWagesFactory : AccountingTransactionFactory
        {
        private:
            double wagesAmount;

        public:
            accounting::Transaction *createTransaction() override;

            EmployeeWagesFactory(util::Date *, std::string, double wagesAmount);

            EmployeeWagesFactory(std::string, double wagesAmount);
        };
    }
}
#endif