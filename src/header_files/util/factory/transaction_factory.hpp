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

            GoodsPurchaseFactory(util::Date *, double, double, double, std::string);

            GoodsPurchaseFactory(double, double, double, std::string);
        };

        class GoodsSellingFactory : AccountingTransactionFactory
        {
        private:
            double sellAmount;
            double paidCash;
            double paidCredit;

        public:
            accounting::Transaction *createTransaction() override;

            GoodsSellingFactory(util::Date *, double, double, double, std::string);

            GoodsSellingFactory(double, double, double, std::string);
        };

        class GoodsSoldCOGS : AccountingTransactionFactory
        {
        private:
            double cogs;

        public:
            accounting::Transaction *createTransaction() override;

            GoodsSoldCOGS(util::Date *, double, std::string);

            GoodsSoldCOGS(double, std::string);
        };
    }
}
#endif