#include "util/database/tables.hpp"

using namespace util;
SellableTable *SellableTable::instance = NULL;
PurchaseTransactionTable *PurchaseTransactionTable::instance = NULL;
PurchaseEntryTable *PurchaseEntryTable::instance = NULL;
SellingEntryTable *SellingEntryTable::instance = NULL;
SellingTransactionTable *SellingTransactionTable::instance = NULL;

// parent class
Table::Table(std::string tableName)
{
    this->tableName = tableName;
}
std::string Table::getTableName()
{
    return this->tableName;
}

std::vector<util::ColumnSchema> Table::getSchema()
{
    return this->schema;
}

std::vector<std::string> Table::insertRow(std::vector<std::string> &values)
{
    int schemaSize = this->getSchema().size();
    int valuesSize = values.size();
    std::string query = "insert into " + this->getTableName() + "(";
    for (int i = 0; i < schemaSize - 1; i++)
    {
        if (this->getSchema()[i].type == util::enums::ColumnTypes::SERIALCOL)
        {
            continue;
        }
        query += this->getSchema()[i].columnName + ", ";
    }
    query += this->getSchema()[schemaSize - 1].columnName + ") values(";
    int curValue = 0;
    for (int i = 0; i < schemaSize - 1; i++)
    {
        switch (this->getSchema()[i].type)
        {
        case util::enums::ColumnTypes::SERIALCOL:
            break;
        case util::enums::ColumnTypes::TEXTCOL:
            query += "'" + values[curValue++] + "', ";
            break;
        case util::enums::ColumnTypes::DATECOL:
            query += "to_date('" + values[curValue++] + "', 'dd-MM-yyyy'), ";
            break;
        case util::enums::ColumnTypes::FLOATCOL:
        case util::enums::ColumnTypes::NUMBERCOL:
            query += values[curValue++] + ", ";
            break;
        }
    }
    switch (this->getSchema()[schemaSize - 1].type)
    {
    case util::enums::ColumnTypes::SERIALCOL:
        break;
    case util::enums::ColumnTypes::TEXTCOL:
        query += "'" + values[curValue++] + "') ";
        break;
    case util::enums::ColumnTypes::DATECOL:
        query += "to_date('" + values[curValue++] + "', 'dd-MM-yyyy')) ";
        break;
    case util::enums::ColumnTypes::FLOATCOL:
    case util::enums::ColumnTypes::NUMBERCOL:
        query += values[curValue++] + ") ";
        break;
    }
    query += "returning *";
    DB *instance = DB::get_instance();
    return instance->execute_query(query)[0];
}

// Sellable Table
SellableTable::SellableTable(std::string tableName) : Table::Table(tableName)
{
    for (auto it = util::enums::sellableTableColumns.begin(); it != util::enums::sellableTableColumns.end(); it++)
    {
        this->schema.push_back(it->second);
    }
}
SellableTable *SellableTable::getInstance()
{
    if (SellableTable::instance == NULL)
    {
        SellableTable::instance = new SellableTable(util::enums::tableNamesMap[util::enums::TableNames::SELLABLE]);
    }
    return SellableTable::instance;
}

// purchase transaction table
PurchaseTransactionTable::PurchaseTransactionTable(std::string tableName) : Table::Table(tableName)
{
    for (auto it = util::enums::purchaseTransactionTableColumns.begin(); it != util::enums::purchaseTransactionTableColumns.end(); it++)
    {
        this->schema.push_back(it->second);
    }
}

PurchaseTransactionTable *PurchaseTransactionTable::getInstance()
{
    if (PurchaseTransactionTable::instance == NULL)
    {
        PurchaseTransactionTable::instance = new PurchaseTransactionTable(util::enums::tableNamesMap[util::enums::TableNames::PURCHASETRANSACTION]);
    }
    return PurchaseTransactionTable::instance;
}

// purchase entry table
PurchaseEntryTable::PurchaseEntryTable(std::string tableName) : Table::Table(tableName)
{
    for (auto it = util::enums::purchaseEntryTableColumns.begin(); it != util::enums::purchaseEntryTableColumns.end(); it++)
    {
        this->schema.push_back(it->second);
    }
}

PurchaseEntryTable *PurchaseEntryTable::getInstance()
{
    if (PurchaseEntryTable::instance == NULL)
    {
        PurchaseEntryTable::instance = new PurchaseEntryTable(util::enums::tableNamesMap[util::enums::TableNames::PURCHASEENTRY]);
    }
    return PurchaseEntryTable::instance;
}

// selling entry table
SellingEntryTable::SellingEntryTable(std::string tableName) : Table::Table(tableName)
{
    for (auto it = util::enums::sellingEntryTableColumns.begin(); it != util::enums::sellingEntryTableColumns.end(); it++)
    {
        this->schema.push_back(it->second);
    }
}
SellingEntryTable *SellingEntryTable::getInstance()
{
    if (SellingEntryTable::instance == NULL)
    {
        SellingEntryTable::instance = new SellingEntryTable(util::enums::tableNamesMap[util::enums::TableNames::SELLINGENTRY]);
    }
    return SellingEntryTable::instance;
}

// selling transaction table
SellingTransactionTable::SellingTransactionTable(std::string tableName) : Table::Table(tableName)
{
    for (auto it = util::enums::sellingTransactionTableColumns.begin(); it != util::enums::sellingTransactionTableColumns.end(); it++)
    {
        this->schema.push_back(it->second);
    }
}
SellingTransactionTable *SellingTransactionTable::getInstance()
{
    if (SellingTransactionTable::instance == NULL)
    {
        SellingTransactionTable::instance = new SellingTransactionTable(util::enums::tableNamesMap[util::enums::TableNames::SELLINGTRANSACTION]);
    }
    return SellingTransactionTable::instance;
}