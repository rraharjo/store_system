#include <vector>
#include <string>
#include "util/enums/database/table_schema.hpp"
#include "util/database/db.hpp"
#ifndef DATABASETABLE_HPP
#define DATABASETABLE_HPP
namespace util
{
    // namespace database
    enum class TableComparator
    {
        LESSTHAN = 0,
        LESSTHANEQUAL,
        EQUAL,
        MORETHANEQUAL,
        MORETHAN,
        LIKE,
        IS,
        ISNOT
    };

    struct TableCondition
    {
        ColumnSchema col;
        std::string value;
        TableComparator comparator;
    };

    class Table
    {
    private:
        std::string table_name;

    protected:
        std::string sequence_name;

        std::vector<ColumnSchema> schema;

        Table(std::string table_name, std::string sequence_name);

    public:
        virtual std::vector<std::vector<std::string>> get_records(std::vector<std::string>, std::vector<TableCondition>);

        virtual std::vector<std::vector<std::string>> get_records(std::vector<TableCondition>);

        virtual std::vector<std::vector<std::string>> get_records_or_conditions(std::vector<TableCondition>);

        std::vector<std::vector<std::string>> get_records();

        virtual std::vector<std::string> insert_row(std::vector<std::string> &values);

        std::vector<std::string> update_row(std::string id, std::vector<std::string> &values);

        std::vector<ColumnSchema> &get_schema();

        std::string get_table_name();

        ColumnSchema get_column(std::string);
    };

    class InventoryTable : public Table
    {
    private:
        static InventoryTable *instance;

    protected:
        InventoryTable(std::string table_name, std::string sequence_name);

    public:
        static InventoryTable *get_instance();

        ~InventoryTable();
    };

    class PurchaseTransactionTable : public Table
    {
    private:
        static PurchaseTransactionTable *instance;

    protected:
        PurchaseTransactionTable(std::string table_name, std::string sequence_name);

    public:
        static PurchaseTransactionTable *get_instance();

        ~PurchaseTransactionTable();
    };

    class PurchaseEntryTable : public Table
    {
    private:
        static PurchaseEntryTable *instance;

    protected:
        PurchaseEntryTable(std::string table_name, std::string sequence_name);

    public:
        static PurchaseEntryTable *get_instance();

        std::vector<std::vector<std::string>> get_records(std::vector<std::string>, std::vector<TableCondition>) override;

        ~PurchaseEntryTable();
    };

    class SellingEntryTable : public Table
    {
    private:
        static SellingEntryTable *instance;

    protected:
        SellingEntryTable(std::string table_name, std::string sequence_name);

    public:
        static SellingEntryTable *get_instance();

        ~SellingEntryTable();
    };

    class SellingTransactionTable : public Table
    {
    private:
        static SellingTransactionTable *instance;

    protected:
        SellingTransactionTable(std::string table_name, std::string sequence_name);

    public:
        static SellingTransactionTable *get_instance();

        ~SellingTransactionTable();
    };

    class AssetsTable : public Table
    {
    private:
        static AssetsTable *instance;

    protected:
        AssetsTable(std::string table_name, std::string sequence_name);

    public:
        static AssetsTable *get_instance();

        ~AssetsTable();
    };

    class AccountingTransactionTable : public Table
    {
    private:
        static AccountingTransactionTable *instance;

    protected:
        AccountingTransactionTable(std::string table_name, std::string sequence_name);

    public:
        static AccountingTransactionTable *get_instance();

        ~AccountingTransactionTable();
    };

    class AccountingEntryTable : public Table
    {
    private:
        static AccountingEntryTable *instance;

    protected:
        AccountingEntryTable(std::string table_name, std::string sequence_name);

    public:
        static AccountingEntryTable *get_instance();

        ~AccountingEntryTable();
    };

    class TAccountTable : public Table
    {
    private:
        static TAccountTable *instance;

    protected:
        TAccountTable(std::string table_name);

    public:
        static TAccountTable *get_instance();

        std::vector<std::string> insert_row(std::vector<std::string> &values) override;

        ~TAccountTable();
    };
}
#endif