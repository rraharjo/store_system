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

    /*****************************************PURCHASETRANSACTION*****************************************/

    class PurchaseTransaction : public Transaction
    {
    private:
        static util::Table *classTable;
        static int nextItemCode;
        std::string seller;

    protected:
        std::vector<std::string> getInsertParameter() override;

        std::string createDBCode() override;

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
        static int nextItemCode;

    public:
        void insertToDB() override;

        void updateToDB() override;
        
        SellingTransaction(util::Date *transactionDate);

    protected:
        std::vector<std::string> getInsertParameter() override;

        std::string createDBCode() override;
    };
};
#endif