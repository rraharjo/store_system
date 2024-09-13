#include "util/enums/database/table_schema.hpp"
using namespace util::enums;
using namespace util;

std::map<TableNames, std::string> util::enums::tableNamesMap = {
    {TableNames::SELLABLE, "sellable"},
    {TableNames::PURCHASEENTRY, "purchase_entry"},
    {TableNames::PURCHASETRANSACTION, "purchase_transaction"},
    {TableNames::SELLINGENTRY, "selling_entry"},
    {TableNames::SELLINGTRANSACTION, "selling_transaction"},
    {TableNames::DEPRECIABLE, "depreciable_table"},
    {TableNames::ACCOUNTINGTRANSACTION, "accounting_transaction"},
    {TableNames::ACCOUNTINGENTRY, "accounting_transaction_entry"}};
/**************************************************************************************************************************/
std::map<SellableTable, ColumnSchema> util::enums::sellableTableColumns = {
    {SellableTable::DATABASECODE, ColumnSchema{"database_code", ColumnTypes::TEXTCOL}},
    {SellableTable::ITEMCODE, ColumnSchema{"item_code", ColumnTypes::TEXTCOL}},
    {SellableTable::ITEMNAME, ColumnSchema{"item_name", ColumnTypes::TEXTCOL}},
    {SellableTable::SELLINGPRICE, ColumnSchema{"selling_price", ColumnTypes::FLOATCOL}}};

std::map<PurchaseEntryTable, ColumnSchema> util::enums::purchaseEntryTableColumns = {
    {PurchaseEntryTable::DATABASECODE, ColumnSchema{"database_code", ColumnTypes::SERIALCOL}},
    {PurchaseEntryTable::SELLABLECODE, ColumnSchema{"sellable_db_code", ColumnTypes::NUMBERCOL}},
    {PurchaseEntryTable::PURCHASETRANSACTIONCODE, ColumnSchema{"purchase_db_code", ColumnTypes::NUMBERCOL}},
    {PurchaseEntryTable::PURCHASEPRICE, ColumnSchema{"purchase_price", ColumnTypes::FLOATCOL}},
    {PurchaseEntryTable::BOUGHTQTY, ColumnSchema{"qty", ColumnTypes::NUMBERCOL}},
    {PurchaseEntryTable::AVAILABLEQTY, ColumnSchema{"available_qty", ColumnTypes::NUMBERCOL}}};

std::map<PurchaseTransactionTable, ColumnSchema> util::enums::purchaseTransactionTableColumns = {
    {PurchaseTransactionTable::DATABASECODE, ColumnSchema{"database_code", ColumnTypes::SERIALCOL}},
    {PurchaseTransactionTable::DATEPURCHASED, ColumnSchema{"date_purchased", ColumnTypes::DATECOL}},
    {PurchaseTransactionTable::SELLER, ColumnSchema{"seller", ColumnTypes::TEXTCOL}}};

std::map<SellingEntryTable, ColumnSchema> util::enums::sellingEntryTableColumns = {
    {SellingEntryTable::DATABASECODE, ColumnSchema{"database_code", ColumnTypes::SERIALCOL}},
    {SellingEntryTable::SELLABLECODE, ColumnSchema{"sellable_db_code", ColumnTypes::NUMBERCOL}},
    {SellingEntryTable::SELLINGTRANSACTIONCODE, ColumnSchema{"selling_transaction_db_code", ColumnTypes::NUMBERCOL}},
    {SellingEntryTable::SELLINGPRICE, ColumnSchema{"selling_price", ColumnTypes::FLOATCOL}},
    {SellingEntryTable::QTY, ColumnSchema{"qty", ColumnTypes::NUMBERCOL}}};

std::map<SellingTransactionTable, ColumnSchema> util::enums::sellingTransactionTableColumns = {
    {SellingTransactionTable::DATABASECODE, ColumnSchema{"database_code", ColumnTypes::SERIALCOL}},
    {SellingTransactionTable::TRANSACTIONDATE, ColumnSchema{"transaction_date", ColumnTypes::DATECOL}}};

std::map<DepreciableTable, ColumnSchema> util::enums::depreciableTableColumns = {
    {DepreciableTable::DATABASECODE, ColumnSchema{"database_code", ColumnTypes::TEXTCOL}},
    {DepreciableTable::ITEMNAME, ColumnSchema{"item_name", ColumnTypes::TEXTCOL}},
    {DepreciableTable::PURCHASECOST, ColumnSchema{"purchase_cost", ColumnTypes::FLOATCOL}},
    {DepreciableTable::RESIDUALVALUE, ColumnSchema{"residual_value", ColumnTypes::FLOATCOL}},
    {DepreciableTable::YEARUSEFULLIFE, ColumnSchema{"year_useful_life", ColumnTypes::NUMBERCOL}},
    {DepreciableTable::DATEPURCHASED, ColumnSchema{"date_purchased", ColumnTypes::DATECOL}},
    {DepreciableTable::DATESOLD, ColumnSchema{"date_sold", ColumnTypes::DATECOL}}};

std::map<AccountingTransactionTable, ColumnSchema> util::enums::accountingTransactionTableColumns = {
    {AccountingTransactionTable::DATABASECODE, ColumnSchema{"database_code", ColumnTypes::SERIALCOL}},
    {AccountingTransactionTable::TRANSACTIONNAME, ColumnSchema{"transaction_name", ColumnTypes::TEXTCOL}},
    {AccountingTransactionTable::TRANSACTIONDATE, ColumnSchema{"transaction_date", ColumnTypes::DATECOL}}};

std::map<AccountingEntryTable, ColumnSchema> util::enums::accountingEntryTableColumns = {
    {AccountingEntryTable::DATABASECODE, ColumnSchema{"database_code", ColumnTypes::SERIALCOL}},
    {AccountingEntryTable::ATDBCODE, ColumnSchema{"at_db_code", ColumnTypes::NUMBERCOL}},
    {AccountingEntryTable::DEBIT, ColumnSchema{"debit", ColumnTypes::BOOLCOL}},
    {AccountingEntryTable::AMOUNT, ColumnSchema{"amount", ColumnTypes::FLOATCOL}},
    {AccountingEntryTable::TACCOUNTNUM, ColumnSchema{"t_account_number", ColumnTypes::TEXTCOL}},
    {AccountingEntryTable::ACCOUNTTITLE, ColumnSchema{"account_title", ColumnTypes::TEXTCOL}}};