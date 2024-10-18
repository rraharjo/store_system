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
            std::string foreignID;

            virtual accounting::Transaction *createTransaction() = 0;

            AccountingTransactionFactory(util::Date *, std::string, std::string);

            virtual ~AccountingTransactionFactory();
        };

        class GoodsPurchaseFactory : public AccountingTransactionFactory
        {
        private:
            double purchaseAmount;
            double paidCash;
            double paidCredit;

        public:
            accounting::Transaction *createTransaction() override;

            GoodsPurchaseFactory(util::Date *, std::string, std::string,
                                 double purchaseAmount, double paidCash, double paidCredit);
        };

        class GoodsSellingFactory : public AccountingTransactionFactory
        {
        private:
            double sellAmount;
            double paidCash;
            double paidCredit;

        public:
            accounting::Transaction *createTransaction() override;

            GoodsSellingFactory &setForeignID(std::string);

            GoodsSellingFactory(util::Date *, std::string, std::string, double sellAmount, double paidCash, double paidCredit);
        };

        class GoodsSoldCOGSFactory : public AccountingTransactionFactory
        {
        private:
            double cogs;

        public:
            accounting::Transaction *createTransaction() override;

            GoodsSoldCOGSFactory(util::Date *, std::string, std::string, double cogs);
        };

        class BuyEquipmentFactory : public AccountingTransactionFactory
        {
        private:
            double equipmentValue;
            double paidCash;
            double paidCredit;

        public:
            accounting::Transaction *createTransaction() override;

            BuyEquipmentFactory(util::Date *, std::string, std::string,
                                double equipmentValue, double paidCash, double paidCredit);
        };

        class SellEquipmentFactory : public AccountingTransactionFactory
        {
        private:
            double accumulatedDepreciation;
            double initialValue;
            double paidCash;
            double paidCredit;

        public:
            accounting::Transaction *createTransaction() override;

            SellEquipmentFactory(util::Date *, std::string, std::string,
                                 double accumulatedDepreciation, double initialValue, double paidCash, double paidCredit);
        };

        class ApplyDepreciationFactory : public AccountingTransactionFactory
        {
        private:
            double depreciationAmount;

        public:
            accounting::Transaction *createTransaction() override;

            ApplyDepreciationFactory(util::Date *, std::string, std::string, double depreciationAmount);
        };

        class EmployeeWagesFactory : public AccountingTransactionFactory
        {
        private:
            double wagesAmount;

        public:
            accounting::Transaction *createTransaction() override;

            EmployeeWagesFactory(util::Date *, std::string, std::string, double wagesAmount);
        };
    }
}
#endif