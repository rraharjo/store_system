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
        std::string column_name;
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

        extern std::map<PrimaryKeyCodes, std::string> primary_key_codes_map;

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

        extern std::map<SequenceNames, std::string> sequence_names_map;

        enum class TableNames
        {
            INVENTORY = 0,
            PURCHASEENTRY,
            PURCHASETRANSACTION,
            SELLINGENTRY,
            SELLINGTRANSACTION,
            ASSETS,
            ACCOUNTINGTRANSACTION,
            ACCOUNTINGENTRY,
            TACCOUNTS
        };

        extern std::map<TableNames, std::string> table_names_map;

        enum class InventoryTable
        {
            DATABASECODE = 0,
            ITEMCODE,
            ITEMNAME,
            SELLINGPRICE
        };

        extern std::map<InventoryTable, ColumnSchema> inventory_table_columns;

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

        extern std::map<PurchaseEntryTable, ColumnSchema> purchase_entry_table_columns;

        enum class PurchaseTransactionTable
        {
            DATABASECODE = 0,
            DATEPURCHASED,
            SELLER,
            PAIDCASH,
            PAIDCREDIT,
            ISFINISHED
        };

        extern std::map<PurchaseTransactionTable, ColumnSchema> purchase_transaction_table_columns;

        enum class SellingEntryTable
        {
            DATABASECODE = 0,
            INVENTORYDBCODE,
            ASSETSCODE,
            SELLINGTRANSACTIONCODE,
            SELLINGPRICE,
            QTY
        };

        extern std::map<SellingEntryTable, ColumnSchema> selling_entry_table_columns;

        enum class SellingTransactionTable
        {
            DATABASECODE = 0,
            TRANSACTIONDATE,
            PAIDCASH,
            PAIDCREDIT,
            ISFINISHED
        };

        extern std::map<SellingTransactionTable, ColumnSchema> selling_transaction_table_columns;

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

        extern std::map<AssetsTable, ColumnSchema> assets_table_columns;

        enum class AccountingTransactionTable
        {
            DATABASECODE = 0,
            TRANSACTIONNAME,
            TRANSACTIONDATE,
            ENTITYID
        };

        extern std::map<AccountingTransactionTable, ColumnSchema> accounting_transaction_table_columns;

        enum class AccountingEntryTable
        {
            DATABASECODE = 0,
            ATDBCODE,
            DEBIT,
            AMOUNT,
            TACCOUNTNUM
        };

        extern std::map<AccountingEntryTable, ColumnSchema> accounting_entry_table_columns;

        enum class TAccountTable{
            TITLE = 0,
            DEBITAMOUNT,
            CREDITAMOUNT
        };

        extern std::map<TAccountTable, ColumnSchema> t_account_table_columns;
    }

}
#endif