#include <string>
#include <vector>
#include "util/date.hpp"
#include "util/database/tables.hpp"
#include "util/class/base_class.hpp"
#include "inventory/transaction/entry.hpp"
#ifndef STORETRANSACTION_HPP
#define STORETRANSACTION_HPP
namespace store
{
    class Transaction : public util::baseclass::HasTable
    {
    private:
        util::Date *transactionDate;
        double paidCash;
        double paidCredit;
        std::vector<inventory::Entry *> entries;

    protected:
        bool isFinished;

        Transaction(util::Date *transactionDate, double paidCash, double paidCredit);

        Transaction(util::Date *transactionDate);

    public:
        util::Date *getDate();

        double getPaidCash();

        double getPaidCredit();

        double getTransactionAmount();

        void addEntry(inventory::Entry *entry);

        void setPaidCash(double amount);

        void setPaidCredit(double amount);

        std::vector<inventory::Entry *> getAllEntries();
    };

    /*****************************************PURCHASETRANSACTION*****************************************/

    class PurchaseTransaction : public Transaction
    {
    private:
        static util::Table *classTable;
        std::string seller;

    protected:
        std::vector<std::string> getInsertParameter() override;

        std::vector<std::string> getUpdateParameter() override;

    public:
        void insertToDB() override;

        void updateToDB() override;

        PurchaseTransaction(std::string seller, util::Date *purchaseDate);

        std::string getSeller();
    };

    /*****************************************SELLINGTRANSACTION*****************************************/

    class SellingTransaction : public Transaction
    {

    private:
        static util::Table *classTable;

    public:
        void insertToDB() override;

        void updateToDB() override;

        SellingTransaction(util::Date *transactionDate);

    protected:
        std::vector<std::string> getInsertParameter() override;

        std::vector<std::string> getUpdateParameter() override;
    };
};
#endif