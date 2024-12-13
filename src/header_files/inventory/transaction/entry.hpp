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
        util::Date *transaction_date;
        std::string sellable_db_code;
        std::string properties_db_code;
        std::string transaction_db_code;
        double price;
        int qty;

    protected:
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
    };

    /**********************************PURCHASEENTRY*********************************/
    class PurchaseEntry : public Entry
    {
    private:
        static util::Table *class_table;

        static std::vector<PurchaseEntry *> generate_from_database(std::string);

        int available_qty;

    protected:
        std::vector<std::string> get_insert_parameter() override;

        std::vector<std::string> get_update_parameter() override;

    public:
        void insert_to_db() override;

        void update_to_db() override;

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
        static util::Table *class_table;
        static int next_item_code;

    protected:
        std::vector<std::string> get_insert_parameter() override;

        std::vector<std::string> get_update_parameter() override;

    public:
        void insert_to_db() override;

        void update_to_db() override;

        SellingEntry(std::string item_db_code, std::string transaction_code, double price, int qty);
    };
}
#endif