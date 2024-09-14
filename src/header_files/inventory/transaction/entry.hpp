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
        util::Date *transactionDate;
        std::string sellableDBCode = "";
        std::string propertiesDBCode = "";
        std::string transactionDBCode;
        double price;
        int qty;

    protected:
        Entry(std::string itemDBCode, std::string transactionCode, double price, int qty);

    public:
        std::string getSellableDBCode();
        std::string getPropertiesDBCode();
        std::string getTransactionDBCode();
        double getPrice();
        int getQty();
        util::Date *getTransactionDate();
        void setTransactionDate(util::Date *transactionDate);
    };

    class PurchaseEntry : public Entry
    {
    private:
        static int nextItemCode;
        int availableQty;

    protected:
        void setTable() override;

        std::vector<std::string> getInsertParameter() override;

        std::string createDBCode() override;

    public:
        PurchaseEntry(std::string itemDBCode, std::string transactionCode, double price, int qty);
        int getAvailableQty();
        void setAvailableQty(int qty);
    };

    class SellingEntry : public Entry
    {
    private:
        static int nextItemCode;

    protected:
        void setTable() override;

        std::vector<std::string> getInsertParameter() override;

        std::string createDBCode() override;

    public:
        SellingEntry(std::string itemDBCode, std::string transactionCode, double price, int qty);
    };
}
#endif