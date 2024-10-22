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
        std::string sellableDBCode;
        std::string propertiesDBCode;
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

    /**********************************PURCHASEENTRY*********************************/
    class PurchaseEntry : public Entry
    {
    private:
        static util::Table *classTable;
        static int nextItemCode;
        int availableQty;

    protected:
        std::vector<std::string> getInsertParameter() override;

        std::vector<std::string> getUpdateParameter() override;

    public:
        void insertToDB() override;

        void updateToDB() override;

        PurchaseEntry(std::string itemDBCode, std::string transactionCode, double price, int qty);

        int getAvailableQty();

        void setAvailableQty(int qty);
    };

    /**********************************SELLINGENTRY*********************************/
    class SellingEntry : public Entry
    {
    private:
        static util::Table *classTable;
        static int nextItemCode;

    protected:
        std::vector<std::string> getInsertParameter() override;

        std::vector<std::string> getUpdateParameter() override;

    public:
        void insertToDB() override;

        void updateToDB() override;

        SellingEntry(std::string itemDBCode, std::string transactionCode, double price, int qty);
    };
}
#endif