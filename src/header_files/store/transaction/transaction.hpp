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
        std::vector<inventory::Entry *> entries;

    protected:
        Transaction(util::Date *transaction_date);
        bool isFinished;

    public:
        util::Date *getDate();

        void addEntry(inventory::Entry *entry);

        std::vector<inventory::Entry *> getAllEntries();
    };

    class PurchaseTransaction : public Transaction
    {
    private:
        static int nextItemCode;
        std::string seller;

    protected:
        void setTable() override;

        std::vector<std::string> getInsertParameter() override;

        std::string createDBCode() override;

    public:
        PurchaseTransaction(std::string seller, util::Date *purchaseDate);

        std::string getSeller();
    };

    class SellingTransaction : public Transaction
    {

    private:
        static int nextItemCode;

    public:
        SellingTransaction(util::Date *transactionDate);

    protected:
        void setTable() override;

        std::vector<std::string> getInsertParameter() override;

        std::string createDBCode() override;
    };
};
#endif