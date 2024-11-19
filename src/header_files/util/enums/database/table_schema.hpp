#include <map>
#include <string>
#ifndef TABLE_SCHEMA_HPP
#define TABLE_SCHEMA_HPP
namespace util
{
    namespace enums
    {
        enum class ColumnTypes
        {
            CUSTOMSERIALCOL = 0, // is a text
            NUMBERCOL,
            FLOATCOL,
            TEXTCOL,
            DATECOL,
            BOOLCOL
        };

    }
    struct ColumnSchema
    {
        std::string columnName;
        util::enums::ColumnTypes type;
    };
    namespace enums
    {
        enum class PrimaryKeyCodes
        {
            INVENTORY = 0,
            PURCHASEENTRY,
            PURCHASETRANSACTION,
            SELLINGENTRY,
            SELLINGTRANSACTION,
            EQUIPMENT,
            ACCOUNTINGTRANSACTION,
            ACCOUNTINGENTRY
        };

        extern std::map<PrimaryKeyCodes, std::string> primaryKeyCodesMap;

        enum class SequenceNames
        {
            INVENTORY = 0,
            PURCHASEENTRY,
            PURCHASETRANSACTION,
            SELLINGENTRY,
            SELLINGTRANSACTION,
            ASSETS,
            ACCOUNTINGTRANSACTION,
            ACCOUNTINGENTRY
        };

        extern std::map<SequenceNames, std::string> sequenceNamesMap;

        enum class TableNames
        {
            INVENTORY = 0,
            PURCHASEENTRY,
            PURCHASETRANSACTION,
            SELLINGENTRY,
            SELLINGTRANSACTION,
            ASSETS,
            ACCOUNTINGTRANSACTION,
            ACCOUNTINGENTRY
        };

        extern std::map<TableNames, std::string> tableNamesMap;

        enum class InventoryTable
        {
            DATABASECODE = 0,
            ITEMCODE,
            ITEMNAME,
            SELLINGPRICE
        };

        extern std::map<InventoryTable, ColumnSchema> inventoryTableColumns;

        enum class PurchaseEntryTable
        {
            DATABASECODE = 0,
            INVENTORYDBCODE,
            ASSETSCODE,
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
            PAIDCASH,
            PAIDCREDIT,
            ISFINISHED
        };

        extern std::map<PurchaseTransactionTable, ColumnSchema> purchaseTransactionTableColumns;

        enum class SellingEntryTable
        {
            DATABASECODE = 0,
            INVENTORYDBCODE,
            ASSETSCODE,
            SELLINGTRANSACTIONCODE,
            SELLINGPRICE,
            QTY
        };

        extern std::map<SellingEntryTable, ColumnSchema> sellingEntryTableColumns;

        enum class SellingTransactionTable
        {
            DATABASECODE = 0,
            TRANSACTIONDATE,
            PAIDCASH,
            PAIDCREDIT,
            ISFINISHED
        };

        extern std::map<SellingTransactionTable, ColumnSchema> sellingTransactionTableColumns;

        enum class AssetsTable
        {
            DATABASECODE = 0,
            ITEMNAME,
            PURCHASECOST,
            RESIDUALVALUE,
            YEARUSEFULLIFE,
            DATEPURCHASED,
            LASTDEPRECIATIONAPPLIED,
            DATESOLD
        };

        extern std::map<AssetsTable, ColumnSchema> assetsTableColumns;

        enum class AccountingTransactionTable
        {
            DATABASECODE = 0,
            TRANSACTIONNAME,
            TRANSACTIONDATE,
            ENTITYID
        };

        extern std::map<AccountingTransactionTable, ColumnSchema> accountingTransactionTableColumns;

        enum class AccountingEntryTable
        {
            DATABASECODE = 0,
            ATDBCODE,
            DEBIT,
            AMOUNT,
            TACCOUNTNUM
        };

        extern std::map<AccountingEntryTable, ColumnSchema> accountingEntryTableColumns;
    }

}
#endif