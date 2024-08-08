#include <map>
#include "util/enums/database/column_schema.hpp"
#ifndef TABLE_SCHEMA_HPP
#define TABLE_SCHEMA_HPP
namespace util
{
    namespace enums
    {
        enum class TableNames
        {
            SELLABLE = 0,
            PURCHASEENTRY,
            PURCHASETRANSACTION,
            SELLINGENTRY,
            SELLINGTRANSACTION
        };

        extern std::map<TableNames, std::string> tableNamesMap;

        enum class SellableTable
        {
            DATABASECODE = 0,
            ITEMCODE,
            ITEMNAME,
            SELLINGPRICE
        };

        extern std::map<SellableTable, ColumnSchema> sellableTableColumns;

        enum class PurchaseEntryTable
        {
            DATABASECODE = 0,
            SELLABLECODE,
            PURCHASETRANSACTIONCODE,
            PURCHASEPRICE,
            BOUGHTQTY,
            AVAILABLEQTY
        };

        extern std::map<PurchaseEntryTable, ColumnSchema> purchaseEntryTableColumns;

        enum class PurchaseTransactionTable
        {
            DATABASECODE = 0,
            DATEPURCHASED,
            SELLER
        };

        extern std::map<PurchaseTransactionTable, ColumnSchema> purchaseTransactionTableColumns;

        enum class SellingEntryTable
        {
            DATABASECODE = 0,
            SELLABLECODE,
            SELLINGTRANSACTIONCODE,
            QTY
        };

        extern std::map<SellingEntryTable, ColumnSchema> sellingEntryTableColumns;

        enum class SellingTransactionTable
        {
            DATABASECODE = 0,
            TRANSACTIONDATE
        };

        extern std::map<SellingTransactionTable, ColumnSchema> sellingTransactionTableColumns;
    }
}
#endif