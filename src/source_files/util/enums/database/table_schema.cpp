#include "util/enums/database/table_schema.hpp"
using namespace util::enums;
using namespace util;

std::map<TableNames, std::string> util::enums::tableNamesMap = {
    {TableNames::SELLABLE, "sellable"},
    {TableNames::PURCHASEENTRY, "purchase_entry"},
    {TableNames::PURCHASETRANSACTION, "purchase_transaction"},
    {TableNames::SELLINGENTRY, "selling_entry"},
    {TableNames::SELLINGTRANSACTION, "selling_transaction"}
};

std::map<SellableTable, ColumnSchema> util::enums::sellableTableColumns = {
    {SellableTable::DATABASECODE, ColumnSchema{"database_code", ColumnTypes::SERIALCOL}},
    {SellableTable::ITEMCODE, ColumnSchema{"item_code", ColumnTypes::TEXTCOL}},
    {SellableTable::ITEMNAME, ColumnSchema{"item_name", ColumnTypes::TEXTCOL}},
    {SellableTable::SELLINGPRICE, ColumnSchema{"selling_price", ColumnTypes::FLOATCOL}}
};

std::map<PurchaseEntryTable, ColumnSchema> util::enums::purchaseEntryTableColumns = {
    {PurchaseEntryTable::DATABASECODE, ColumnSchema{"database_code", ColumnTypes::SERIALCOL}},
    {PurchaseEntryTable::SELLABLECODE, ColumnSchema{"sellable_db_code", ColumnTypes::NUMBERCOL}},
    {PurchaseEntryTable::PURCHASETRANSACTIONCODE, ColumnSchema{"purchase_db_code", ColumnTypes::NUMBERCOL}},
    {PurchaseEntryTable::PURCHASEPRICE, ColumnSchema{"purchase_price", ColumnTypes::FLOATCOL}},
    {PurchaseEntryTable::BOUGHTQTY, ColumnSchema{"qty", ColumnTypes::NUMBERCOL}},
    {PurchaseEntryTable::AVAILABLEQTY, ColumnSchema{"available_qty", ColumnTypes::NUMBERCOL}}
};

std::map<PurchaseTransactionTable, ColumnSchema> util::enums::purchaseTransactionTableColumns = {
    {PurchaseTransactionTable::DATABASECODE, ColumnSchema{"database_code", ColumnTypes::SERIALCOL}},
    {PurchaseTransactionTable::DATEPURCHASED, ColumnSchema{"date_purchased", ColumnTypes::DATECOL}},
    {PurchaseTransactionTable::SELLER, ColumnSchema{"seller", ColumnTypes::TEXTCOL}}
};

std::map<SellingEntryTable, ColumnSchema> util::enums::sellingEntryTableColumns = {
    {SellingEntryTable::DATABASECODE, ColumnSchema{"database_code", ColumnTypes::SERIALCOL}},
    {SellingEntryTable::SELLABLECODE, ColumnSchema{"sellable_db_code", ColumnTypes::NUMBERCOL}},
    {SellingEntryTable::SELLINGTRANSACTIONCODE, ColumnSchema{"selling_transaction_db_code", ColumnTypes::NUMBERCOL}},
    {SellingEntryTable::QTY, ColumnSchema{"qty", ColumnTypes::NUMBERCOL}}
};

std::map<SellingTransactionTable, ColumnSchema> util::enums::sellingTransactionTableColumns = {
    {SellingTransactionTable::DATABASECODE, ColumnSchema{"database_code", ColumnTypes::SERIALCOL}},
    {SellingTransactionTable::TRANSACTIONDATE, ColumnSchema{"transaction_date", ColumnTypes::DATECOL}}  
};