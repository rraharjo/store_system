#include <vector>
#include <string>
#include "util/enums/database/table_schema.hpp"
#include "util/database/db.hpp"
#ifndef DATABASETABLE_HPP
#define DATABASETABLE_HPP
namespace util
{
    //namespace database
    class Table
    {
    private:
        std::string tableName;

    protected:
        std::vector<ColumnSchema> schema;
        Table(std::string tableName);

    public:
        std::vector<std::string> insertRow(std::vector<std::string> &values);

        std::vector<ColumnSchema> getSchema();

        std::string getTableName();
    };

    class SellableTable : public Table
    {
    private:
        static SellableTable *instance;

    protected:
        SellableTable(std::string tableName);

    public:
        static SellableTable *getInstance();
    };

    class PurchaseTransactionTable : public Table
    {
    private:
        static PurchaseTransactionTable *instance;

    protected:
        PurchaseTransactionTable(std::string tableName);

    public:
        static PurchaseTransactionTable *getInstance();
    };

    class PurchaseEntryTable : public Table
    {
    private:
        static PurchaseEntryTable *instance;

    protected:
        PurchaseEntryTable(std::string tableName);

    public:
        static PurchaseEntryTable *getInstance();
    };

    class SellingEntryTable : public Table
    {
    private:
        static SellingEntryTable *instance;

    protected:
        SellingEntryTable(std::string tableName);

    public:
        static SellingEntryTable *getInstance();
    };

    class SellingTransactionTable : public Table
    {
    private:
        static SellingTransactionTable *instance;

    protected:
        SellingTransactionTable(std::string tableName);

    public:
        static SellingTransactionTable *getInstance();
    };
};
#endif