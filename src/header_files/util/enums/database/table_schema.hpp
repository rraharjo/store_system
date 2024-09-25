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
            SELLINGTRANSACTION,
            DEPRECIABLE,
            ACCOUNTINGTRANSACTION,
            ACCOUNTINGENTRY
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
            PROPERTIESCODE,
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
            SELLER,
            ISFINISHED
        };

        extern std::map<PurchaseTransactionTable, ColumnSchema> purchaseTransactionTableColumns;

        enum class SellingEntryTable
        {
            DATABASECODE = 0,
            SELLABLECODE,
            PROPERTIESCODE,
            SELLINGTRANSACTIONCODE,
            SELLINGPRICE,
            QTY
        };

        extern std::map<SellingEntryTable, ColumnSchema> sellingEntryTableColumns;

        enum class SellingTransactionTable
        {
            DATABASECODE = 0,
            TRANSACTIONDATE,
            ISFINISHED
        };

        extern std::map<SellingTransactionTable, ColumnSchema> sellingTransactionTableColumns;

        enum class DepreciableTable
        {
            DATABASECODE = 0,
            ITEMNAME,
            PURCHASECOST,
            RESIDUALVALUE,
            YEARUSEFULLIFE,
            DATEPURCHASED,
            DATESOLD
        };

        extern std::map<DepreciableTable, ColumnSchema> depreciableTableColumns;

        enum class AccountingTransactionTable
        {
            DATABASECODE = 0,
            TRANSACTIONNAME,
            TRANSACTIONDATE,
            PURCHASETID,
            SELLTID
        };

        extern std::map<AccountingTransactionTable, ColumnSchema> accountingTransactionTableColumns;

        enum class AccountingEntryTable
        {
            DATABASECODE = 0,
            ATDBCODE,
            DEBIT,
            AMOUNT,
            TACCOUNTNUM,
            ACCOUNTTITLE
        };

        extern std::map<AccountingEntryTable, ColumnSchema> accountingEntryTableColumns;
    }
}
#endif