#include <string>
#include <vector>
#include "util/date.hpp"
#include "util/database/tables.hpp"
#include "util/class/base_class.hpp"
#include "../constants/structs.hpp"
#ifndef TRANSACTIONINVENTORY_HPP
#define TRANSACTIONINVENTORY_HPP
namespace inventory
{
    class Transaction : public util::baseclass::HasTable
    {
    private:
        util::Date *transactionDate;
        
        std::vector<TransactionEntry *> entries;

    public:
        Transaction(util::Date *transaction_date);

        util::Date *getDate();

        void addEntry(int itemDBCode, int qty, double price);
    };

    class PurchaseTransaction : public Transaction
    {
    private:
        std::string seller;

    protected:
        void setTable() override;

    public:
        PurchaseTransaction(std::string seller, util::Date *purchaseDate);
    };
    
    class SellingTransaction : public Transaction
    {

    public:
        SellingTransaction(util::Date *transactionDate);

    protected:
        void setTable() override;
    };
};
#endif