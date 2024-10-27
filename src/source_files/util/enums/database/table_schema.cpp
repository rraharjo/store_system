#include "util/enums/database/table_schema.hpp"
using namespace util::enums;
using namespace util;

std::map<PrimaryKeyCodes, std::string> util::enums::primaryKeyCodesMap = {
    {PrimaryKeyCodes::INVENTORY, "INV"},
    {PrimaryKeyCodes::PURCHASEENTRY, "PNT"},
    {PrimaryKeyCodes::PURCHASETRANSACTION, "PTR"},
    {PrimaryKeyCodes::SELLINGENTRY, "SNT"},
    {PrimaryKeyCodes::SELLINGTRANSACTION, "STR"},
    {PrimaryKeyCodes::EQUIPMENT, "EQP"},
    {PrimaryKeyCodes::ACCOUNTINGTRANSACTION, "ATR"},
    {PrimaryKeyCodes::ACCOUNTINGENTRY, "ANT"}};

std::map<SequenceNames, std::string> util::enums::sequenceNamesMap = {
    {SequenceNames::INVENTORY, "inventory_seq"},
    {SequenceNames::PURCHASEENTRY, "purchase_entry_seq"},
    {SequenceNames::PURCHASETRANSACTION, "purchase_transaction_seq"},
    {SequenceNames::SELLINGENTRY, "selling_entry_seq"},
    {SequenceNames::SELLINGTRANSACTION, "selling_transaction_seq"},
    {SequenceNames::ASSETS, "assets_seq"},
    {SequenceNames::ACCOUNTINGTRANSACTION, "accounting_transaction_seq"},
    {SequenceNames::ACCOUNTINGENTRY, "accounting_transaction_entry_seq"}};

std::map<TableNames, std::string> util::enums::tableNamesMap = {
    {TableNames::INVENTORY, "inventory"},
    {TableNames::PURCHASEENTRY, "purchase_entry"},
    {TableNames::PURCHASETRANSACTION, "purchase_transaction"},
    {TableNames::SELLINGENTRY, "selling_entry"},
    {TableNames::SELLINGTRANSACTION, "selling_transaction"},
    {TableNames::ASSETS, "assets"},
    {TableNames::ACCOUNTINGTRANSACTION, "accounting_transaction"},
    {TableNames::ACCOUNTINGENTRY, "accounting_transaction_entry"}};
/**************************************************************************************************************************/
std::map<InventoryTable, ColumnSchema> util::enums::inventoryTableColumns = {
    {InventoryTable::DATABASECODE, ColumnSchema{"database_code", ColumnTypes::CUSTOMSERIALCOL}},
    {InventoryTable::ITEMCODE, ColumnSchema{"item_code", ColumnTypes::TEXTCOL}},
    {InventoryTable::ITEMNAME, ColumnSchema{"item_name", ColumnTypes::TEXTCOL}},
    {InventoryTable::SELLINGPRICE, ColumnSchema{"selling_price", ColumnTypes::FLOATCOL}}};

std::map<PurchaseEntryTable, ColumnSchema> util::enums::purchaseEntryTableColumns = {
    {PurchaseEntryTable::DATABASECODE, ColumnSchema{"database_code", ColumnTypes::CUSTOMSERIALCOL}},
    {PurchaseEntryTable::INVENTORYDBCODE, ColumnSchema{"inventory_db_code", ColumnTypes::TEXTCOL}},
    {PurchaseEntryTable::ASSETSCODE, ColumnSchema{"assets_db_code", ColumnTypes::TEXTCOL}},
    {PurchaseEntryTable::PURCHASETRANSACTIONCODE, ColumnSchema{"purchase_db_code", ColumnTypes::TEXTCOL}},
    {PurchaseEntryTable::PURCHASEPRICE, ColumnSchema{"purchase_price", ColumnTypes::FLOATCOL}},
    {PurchaseEntryTable::BOUGHTQTY, ColumnSchema{"qty", ColumnTypes::NUMBERCOL}},
    {PurchaseEntryTable::AVAILABLEQTY, ColumnSchema{"available_qty", ColumnTypes::NUMBERCOL}}};

std::map<PurchaseTransactionTable, ColumnSchema> util::enums::purchaseTransactionTableColumns = {
    {PurchaseTransactionTable::DATABASECODE, ColumnSchema{"database_code", ColumnTypes::CUSTOMSERIALCOL}},
    {PurchaseTransactionTable::DATEPURCHASED, ColumnSchema{"date_purchased", ColumnTypes::DATECOL}},
    {PurchaseTransactionTable::SELLER, ColumnSchema{"seller", ColumnTypes::TEXTCOL}},
    {PurchaseTransactionTable::ISFINISHED, ColumnSchema{"is_finished", ColumnTypes::BOOLCOL}}};

std::map<SellingEntryTable, ColumnSchema> util::enums::sellingEntryTableColumns = {
    {SellingEntryTable::DATABASECODE, ColumnSchema{"database_code", ColumnTypes::CUSTOMSERIALCOL}},
    {SellingEntryTable::INVENTORYDBCODE, ColumnSchema{"inventory_db_code", ColumnTypes::TEXTCOL}},
    {SellingEntryTable::ASSETSCODE, ColumnSchema{"assets_db_code", ColumnTypes::TEXTCOL}},
    {SellingEntryTable::SELLINGTRANSACTIONCODE, ColumnSchema{"selling_transaction_db_code", ColumnTypes::TEXTCOL}},
    {SellingEntryTable::SELLINGPRICE, ColumnSchema{"selling_price", ColumnTypes::FLOATCOL}},
    {SellingEntryTable::QTY, ColumnSchema{"qty", ColumnTypes::NUMBERCOL}}};

std::map<SellingTransactionTable, ColumnSchema> util::enums::sellingTransactionTableColumns = {
    {SellingTransactionTable::DATABASECODE, ColumnSchema{"database_code", ColumnTypes::CUSTOMSERIALCOL}},
    {SellingTransactionTable::TRANSACTIONDATE, ColumnSchema{"transaction_date", ColumnTypes::DATECOL}},
    {SellingTransactionTable::ISFINISHED, ColumnSchema{"is_finished", ColumnTypes::BOOLCOL}}};

std::map<AssetsTable, ColumnSchema> util::enums::assetsTableColumns = {
    {AssetsTable::DATABASECODE, ColumnSchema{"database_code", ColumnTypes::CUSTOMSERIALCOL}},
    {AssetsTable::ITEMNAME, ColumnSchema{"item_name", ColumnTypes::TEXTCOL}},
    {AssetsTable::PURCHASECOST, ColumnSchema{"purchase_cost", ColumnTypes::FLOATCOL}},
    {AssetsTable::RESIDUALVALUE, ColumnSchema{"residual_value", ColumnTypes::FLOATCOL}},
    {AssetsTable::YEARUSEFULLIFE, ColumnSchema{"year_useful_life", ColumnTypes::NUMBERCOL}},
    {AssetsTable::DATEPURCHASED, ColumnSchema{"date_purchased", ColumnTypes::DATECOL}},
    {AssetsTable::DATESOLD, ColumnSchema{"date_sold", ColumnTypes::DATECOL}}};

std::map<AccountingTransactionTable, ColumnSchema> util::enums::accountingTransactionTableColumns = {
    {AccountingTransactionTable::DATABASECODE, ColumnSchema{"database_code", ColumnTypes::CUSTOMSERIALCOL}},
    {AccountingTransactionTable::TRANSACTIONNAME, ColumnSchema{"transaction_name", ColumnTypes::TEXTCOL}},
    {AccountingTransactionTable::TRANSACTIONDATE, ColumnSchema{"transaction_date", ColumnTypes::DATECOL}},
    {AccountingTransactionTable::ENTITYID, ColumnSchema{"entity_id", ColumnTypes::TEXTCOL}}};

std::map<AccountingEntryTable, ColumnSchema> util::enums::accountingEntryTableColumns = {
    {AccountingEntryTable::DATABASECODE, ColumnSchema{"database_code", ColumnTypes::CUSTOMSERIALCOL}},
    {AccountingEntryTable::ATDBCODE, ColumnSchema{"at_db_code", ColumnTypes::TEXTCOL}},
    {AccountingEntryTable::DEBIT, ColumnSchema{"debit", ColumnTypes::BOOLCOL}},
    {AccountingEntryTable::AMOUNT, ColumnSchema{"amount", ColumnTypes::FLOATCOL}},
    {AccountingEntryTable::TACCOUNTNUM, ColumnSchema{"t_account_number", ColumnTypes::TEXTCOL}},
    {AccountingEntryTable::ACCOUNTTITLE, ColumnSchema{"account_title", ColumnTypes::TEXTCOL}}};