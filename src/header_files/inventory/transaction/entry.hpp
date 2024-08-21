#include "util/class/base_class.hpp"
#include "util/database/tables.hpp"
#include "util/date.hpp"
#ifndef INVENTORYENTRY_HPP
#define INVENTORYENTRY_HPP
namespace inventory
{
    class Entry : public util::baseclass::HasTable
    {
    private:
        util::Date* transactionDate;
        int sellableDBCode;
        int transactionDBCode;
        double price;
        int qty;

    protected:
        Entry(int sellableDBCode, int transactionCode, double price, int qty);

    public:
        int getSellableDBCode();
        int getTransactionDBCode();
        double getPrice();
        int getQty();
        util::Date* getTransactionDate();
        void setTransactionDate(util::Date* transactionDate);
    };

    class PurchaseEntry : public Entry
    {
    private:
        int availableQty;

    protected:
        void setTable() override;

        std::vector<std::string> getInsertParameter() override;

    public:
        PurchaseEntry(int sellableDBCode, int transactionCode, double price, int qty);
        int getAvailableQty();
        void setAvailableQty(int qty);
    };

    class SellingEntry : public Entry
    {
    protected:
        void setTable() override;

        std::vector<std::string> getInsertParameter() override;

    public:
        SellingEntry(int sellableDBCode, int transactionCode, double price, int qty);
    };
}
#endif