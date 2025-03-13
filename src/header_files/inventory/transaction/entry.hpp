#include "util/class/base_class.hpp"
#include "util/database/tables.hpp"
#include "util/date.hpp"
#ifndef INVENTORYENTRY_HPP
#define INVENTORYENTRY_HPP
namespace util{
    namespace baseclass{
        class PurchaseEntriesCollection;
        class SellingEntriesCollection;
    }
}
namespace inventory
{
    class Entry : public util::baseclass::HasTable
    {
    private:
        util::Date *transaction_date;
        std::string sellable_db_code;
        std::string properties_db_code;
        std::string transaction_db_code;
        double price;
        int qty;

    protected:
        void set_item_db_code(std::string db_code);

        Entry(std::string item_db_code, std::string transaction_code, double price, int qty);

    public:
        std::string get_sellable_db_code();
        std::string get_properties_db_code();
        std::string get_transaction_db_code();
        double get_price();
        int get_qty();
        util::Date *get_transaction_date();

        void set_transaction_date(util::Date *transaction_date);

        void set_transaction_db_code(std::string db_code);

        friend class util::baseclass::PurchaseEntriesCollection;
        friend class util::baseclass::SellingEntriesCollection;
    };

    /**********************************PURCHASEENTRY*********************************/
    class PurchaseEntry : public Entry
    {
    private:
        int available_qty;

    public:
        PurchaseEntry(std::string db_code, std::string item_db_code, std::string transaction_db_code,
                      double price, int all_qty, int available_qty);

        PurchaseEntry(std::string item_db_code, std::string transaction_db_code, double price, int qty);

        int get_available_qty();

        void set_available_qty(int qty);

        friend class Inventory;

        friend class Equipment;
    };

    /**********************************SELLINGENTRY*********************************/
    class SellingEntry : public Entry
    {
    private:
        static int next_item_code;

    public:
        SellingEntry(std::string item_db_code, std::string transaction_code, double price, int qty);
        SellingEntry(std::string db_code, std::string item_db_code, std::string transaction_code, double price, int qty);
    };
}
#endif