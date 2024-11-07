#include <vector>
#include <string>
#include "util/enums/database/table_schema.hpp"
#include "util/database/db.hpp"
#ifndef DATABASETABLE_HPP
#define DATABASETABLE_HPP
namespace util
{
    // namespace database
    enum class TableComparator{
        LESSTHAN = 0,
        LESSTHANEQUAL,
        EQUAL,
        MORETHANEQUAL,
        MORETHAN,
        LIKE
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
        std::string tableName;

    protected:
        std::string sequenceName;

        std::vector<ColumnSchema> schema;

        Table(std::string tableName, std::string sequenceName);

    public:
        virtual std::vector<std::vector<std::string>> getRecords(std::vector<std::string>, std::vector<TableCondition>);

        virtual std::vector<std::vector<std::string>> getRecords(std::vector<TableCondition>);

        std::vector<std::vector<std::string>> getRecords();

        std::vector<std::string> insertRow(std::vector<std::string> &values);

        std::vector<std::string> updateRow(std::string id, std::vector<std::string> &values);

        std::vector<ColumnSchema> getSchema();

        std::string getTableName();

        ColumnSchema getColumn(std::string);
    };

    class InventoryTable : public Table
    {
    private:
        static InventoryTable *instance;

    protected:
        InventoryTable(std::string tableName, std::string sequenceName);

    public:
        static InventoryTable *getInstance();

        ~InventoryTable();
    };

    class PurchaseTransactionTable : public Table
    {
    private:
        static PurchaseTransactionTable *instance;

    protected:
        PurchaseTransactionTable(std::string tableName, std::string sequenceName);

    public:
        static PurchaseTransactionTable *getInstance();

        ~PurchaseTransactionTable();
    };

    class PurchaseEntryTable : public Table
    {
    private:
        static PurchaseEntryTable *instance;

    protected:
        PurchaseEntryTable(std::string tableName, std::string sequenceName);

    public:
        static PurchaseEntryTable *getInstance();

        std::vector<std::vector<std::string>> getRecords(std::vector<std::string>, std::vector<TableCondition>) override;

        ~PurchaseEntryTable();
    };

    class SellingEntryTable : public Table
    {
    private:
        static SellingEntryTable *instance;

    protected:
        SellingEntryTable(std::string tableName, std::string sequenceName);

    public:
        static SellingEntryTable *getInstance();

        ~SellingEntryTable();
    };

    class SellingTransactionTable : public Table
    {
    private:
        static SellingTransactionTable *instance;

    protected:
        SellingTransactionTable(std::string tableName, std::string sequenceName);

    public:
        static SellingTransactionTable *getInstance();

        ~SellingTransactionTable();
    };

    class AssetsTable : public Table
    {
    private:
        static AssetsTable *instance;

    protected:
        AssetsTable(std::string tableName, std::string sequenceName);

    public:
        static AssetsTable *getInstance();

        ~AssetsTable();
    };

    class AccountingTransactionTable : public Table
    {
    private:
        static AccountingTransactionTable *instance;

    protected:
        AccountingTransactionTable(std::string tableName, std::string sequenceName);

    public:
        static AccountingTransactionTable *getInstance();

        ~AccountingTransactionTable();
    };

    class AccountingEntryTable : public Table
    {
    private:
        static AccountingEntryTable *instance;

    protected:
        AccountingEntryTable(std::string tableName, std::string sequenceName);

    public:
        static AccountingEntryTable *getInstance();

        ~AccountingEntryTable();
    };
};
#endif