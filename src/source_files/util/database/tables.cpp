#include "util/database/tables.hpp"

using namespace util;
SellableTable *SellableTable::instance = NULL;
PurchaseTransactionTable *PurchaseTransactionTable::instance = NULL;
PurchaseEntryTable *PurchaseEntryTable::instance = NULL;
SellingEntryTable *SellingEntryTable::instance = NULL;
SellingTransactionTable *SellingTransactionTable::instance = NULL;
AccountingTransactionTable *AccountingTransactionTable::instance = NULL;
AccountingEntryTable *AccountingEntryTable::instance = NULL;
DepreciableTable *DepreciableTable::instance = NULL;

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
        if (values[curValue] == "NULL")
        {
            query += "NULL,";
            curValue++;
            continue;
        }
        switch (this->getSchema()[i].type)
        {
        case util::enums::ColumnTypes::SERIALCOL:
            break;
        case util::enums::ColumnTypes::TEXTCOL:
            query += "'" + values[curValue++] + "',";
            break;
        case util::enums::ColumnTypes::DATECOL:
            query += "to_date('" + values[curValue++] + "', 'dd-MM-yyyy'),";
            break;
        case util::enums::ColumnTypes::BOOLCOL:
            query += std::stoi(values[curValue++]) ? "true," : "false,";
            break;
        case util::enums::ColumnTypes::FLOATCOL:
        case util::enums::ColumnTypes::NUMBERCOL:
            query += values[curValue++] + ",";
            break;
        }
    }
    if (values[curValue] == "NULL")
    {
        query += "NULL)";
    }
    else
    {
        switch (this->getSchema()[schemaSize - 1].type)
        {
        case util::enums::ColumnTypes::SERIALCOL:
            break;
        case util::enums::ColumnTypes::TEXTCOL:
            query += "'" + values[curValue++] + "')";
            break;
        case util::enums::ColumnTypes::DATECOL:
            query += "to_date('" + values[curValue++] + "', 'dd-MM-yyyy'))";
            break;
        case util::enums::ColumnTypes::BOOLCOL:
            query += std::stoi(values[curValue++]) ? "true)" : "false)";
            break;
        case util::enums::ColumnTypes::FLOATCOL:
        case util::enums::ColumnTypes::NUMBERCOL:
            query += values[curValue++] + ")";
            break;
        }
    }

    query += "returning *";
    DB *instance = DB::get_instance();
    return instance->execute_query(query)[0];
}

std::vector<std::string> Table::updateRow(std::string id, std::vector<std::string> &values)
{
    int schemaSize = this->getSchema().size();
    std::string query = "update " + this->tableName + " set ";
    int curValue = 0;
    for (int i = 0; i < schemaSize - 1; i++)
    {
        util::ColumnSchema currentCol = this->getSchema()[i];
        switch (currentCol.type)
        {
        case util::enums::ColumnTypes::SERIALCOL:
            break;
        case util::enums::ColumnTypes::TEXTCOL:
            query += currentCol.columnName + " = ";
            query += "'" + values[curValue++] + "',";
            break;
        case util::enums::ColumnTypes::DATECOL:
            query += currentCol.columnName + " = ";
            query += "to_date('" + values[curValue++] + "', 'dd-MM-yyyy'),";
            break;
        case util::enums::ColumnTypes::BOOLCOL:
            query += currentCol.columnName + " = ";
            query += std::stoi(values[curValue++]) ? "true," : "false,";
            break;
        case util::enums::ColumnTypes::FLOATCOL:
        case util::enums::ColumnTypes::NUMBERCOL:
            query += currentCol.columnName + " = ";
            query += values[curValue++] + ",";
            break;
        }
    }
    util::ColumnSchema currentCol = this->getSchema()[schemaSize - 1];
    query += currentCol.columnName + " = ";
    switch (currentCol.type)
    {
    case util::enums::ColumnTypes::SERIALCOL:
        break;
    case util::enums::ColumnTypes::TEXTCOL:
        query += "'" + values[curValue++] + "' ";
        break;
    case util::enums::ColumnTypes::DATECOL:
        query += "to_date('" + values[curValue++] + "', 'dd-MM-yyyy') ";
        break;
    case util::enums::ColumnTypes::BOOLCOL:
        query += std::stoi(values[curValue++]) ? "true " : "false ";
        break;
    case util::enums::ColumnTypes::FLOATCOL:
    case util::enums::ColumnTypes::NUMBERCOL:
        query += values[curValue++] + " ";
        break;
    }
    query += "where " + this->getSchema()[0].columnName + " = '" + id + "' returning *;";
    // std::cout << query << std::endl;
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

SellableTable::~SellableTable()
{
    SellableTable::instance = NULL;
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

PurchaseTransactionTable::~PurchaseTransactionTable()
{
    PurchaseTransactionTable::instance = NULL;
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

PurchaseEntryTable::~PurchaseEntryTable()
{
    PurchaseEntryTable::instance = NULL;
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

SellingEntryTable::~SellingEntryTable()
{
    SellingEntryTable::instance = NULL;
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

SellingTransactionTable::~SellingTransactionTable()
{
    SellingTransactionTable::instance = NULL;
}

SellingTransactionTable *SellingTransactionTable::getInstance()
{
    if (SellingTransactionTable::instance == NULL)
    {
        SellingTransactionTable::instance = new SellingTransactionTable(util::enums::tableNamesMap[util::enums::TableNames::SELLINGTRANSACTION]);
    }
    return SellingTransactionTable::instance;
}

// Depreciable Table
DepreciableTable::DepreciableTable(std::string tableName) : Table::Table(tableName)
{
    for (auto it = util::enums::depreciableTableColumns.begin(); it != util::enums::depreciableTableColumns.end(); it++)
    {
        this->schema.push_back(it->second);
    }
}

DepreciableTable::~DepreciableTable()
{
    DepreciableTable::instance = NULL;
}

DepreciableTable *DepreciableTable::getInstance()
{
    if (DepreciableTable::instance == NULL)
    {
        DepreciableTable::instance = new DepreciableTable(util::enums::tableNamesMap[util::enums::TableNames::DEPRECIABLE]);
    }
    return DepreciableTable::instance;
}

// accounting transaction table
AccountingTransactionTable::AccountingTransactionTable(std::string tableName) : Table::Table(tableName)
{
    for (auto it = util::enums::accountingTransactionTableColumns.begin(); it != util::enums::accountingTransactionTableColumns.end(); it++)
    {
        this->schema.push_back(it->second);
    }
}

AccountingTransactionTable::~AccountingTransactionTable()
{
    AccountingTransactionTable::instance = NULL;
}

AccountingTransactionTable *AccountingTransactionTable::getInstance()
{
    if (AccountingTransactionTable::instance == NULL)
    {
        AccountingTransactionTable::instance = new AccountingTransactionTable(util::enums::tableNamesMap[util::enums::TableNames::ACCOUNTINGTRANSACTION]);
    }
    return AccountingTransactionTable::instance;
}

// accounting entry table
AccountingEntryTable::AccountingEntryTable(std::string tableName) : Table::Table(tableName)
{
    for (auto it = util::enums::accountingEntryTableColumns.begin(); it != util::enums::accountingEntryTableColumns.end(); it++)
    {
        this->schema.push_back(it->second);
    }
}

AccountingEntryTable::~AccountingEntryTable()
{
    std::cout << "destructor called" << std::endl;
    AccountingEntryTable::instance = NULL;
}

AccountingEntryTable *AccountingEntryTable::getInstance()
{
    if (AccountingEntryTable::instance == NULL)
    {
        AccountingEntryTable::instance = new AccountingEntryTable(util::enums::tableNamesMap[util::enums::TableNames::ACCOUNTINGENTRY]);
    }
    return AccountingEntryTable::instance;
}