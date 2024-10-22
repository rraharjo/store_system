#include "util/database/tables.hpp"

using namespace util;
InventoryTable *InventoryTable::instance = NULL;
PurchaseTransactionTable *PurchaseTransactionTable::instance = NULL;
PurchaseEntryTable *PurchaseEntryTable::instance = NULL;
SellingEntryTable *SellingEntryTable::instance = NULL;
SellingTransactionTable *SellingTransactionTable::instance = NULL;
AccountingTransactionTable *AccountingTransactionTable::instance = NULL;
AccountingEntryTable *AccountingEntryTable::instance = NULL;
AssetsTable *AssetsTable::instance = NULL;

// parent class
Table::Table(std::string tableName, std::string sequenceName)
{
    this->tableName = tableName;
    this->sequenceName = sequenceName;
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
        /*if (this->getSchema()[i].type == util::enums::ColumnTypes::SERIALCOL)
        {
            continue;
        }*/
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
            query += "concat('" + values[curValue++] + "', lpad(nextval('" + this->sequenceName + "')::text, 6, '0')), ";
            break;
        case util::enums::ColumnTypes::TEXTCOL:
            query += "'" + values[curValue++] + "',";
            break;
        case util::enums::ColumnTypes::DATECOL:
            query += "to_date('" + values[curValue++] + "', 'dd-MM-yyyy'),";
            break;
        case util::enums::ColumnTypes::BOOLCOL:
            query += values[curValue++] + ",";
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
            query += "concat('" + values[curValue++] + "', lpad(nextval('" + this->sequenceName + "')::text, 6, '0'))";
            break;
        case util::enums::ColumnTypes::TEXTCOL:
            query += "'" + values[curValue++] + "')";
            break;
        case util::enums::ColumnTypes::DATECOL:
            query += "to_date('" + values[curValue++] + "', 'dd-MM-yyyy'))";
            break;
        case util::enums::ColumnTypes::BOOLCOL:
            query += values[curValue++] + ")"; // Error
            break;
        case util::enums::ColumnTypes::FLOATCOL:
        case util::enums::ColumnTypes::NUMBERCOL:
            query += values[curValue++] + ")";
            break;
        }
    }

    query += "returning *;";
    DB *instance = DB::getInstance();
    return instance->executeQuery(query)[0];
}

std::vector<std::string> Table::updateRow(std::string id, std::vector<std::string> &values)
{
    int schemaSize = this->getSchema().size();
    std::string query = "update " + this->tableName + " set ";
    int curValue = 0;
    for (int i = 0; i < schemaSize - 1; i++)
    {
        util::ColumnSchema currentCol = this->getSchema()[i];
        if (currentCol.type == util::enums::ColumnTypes::SERIALCOL)
        {
            continue;
        }
        if (values[curValue] == "NULL")
        {
            query += currentCol.columnName + " = NULL,";
            curValue++;
            continue;
        }

        switch (currentCol.type)
        {
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
    if (currentCol.type == util::enums::ColumnTypes::SERIALCOL)
    {
        query[query.length() - 1] = ' ';
    }
    else if (values[curValue] == "NULL")
    {
        query += currentCol.columnName + " = NULL ";
        curValue++;
    }
    else
    {
        switch (currentCol.type)
        {
        case util::enums::ColumnTypes::TEXTCOL:
            query += currentCol.columnName + " = ";
            query += "'" + values[curValue++] + "' ";
            break;
        case util::enums::ColumnTypes::DATECOL:
            query += currentCol.columnName + " = ";
            query += "to_date('" + values[curValue++] + "', 'dd-MM-yyyy') ";
            break;
        case util::enums::ColumnTypes::BOOLCOL:
            query += currentCol.columnName + " = ";
            query += std::stoi(values[curValue++]) ? "true " : "false ";
            break;
        case util::enums::ColumnTypes::FLOATCOL:
        case util::enums::ColumnTypes::NUMBERCOL:
            query += currentCol.columnName + " = ";
            query += values[curValue++] + " ";
            break;
        }
    }
    query += "where " + this->getSchema()[0].columnName + " = '" + id + "' returning *;";
    // std::cout << query << std::endl;
    DB *instance = DB::getInstance();
    return instance->executeQuery(query)[0];
}

// Inventory Table
InventoryTable::InventoryTable(std::string tableName, std::string sequenceName) : Table::Table(tableName, sequenceName)
{
    for (auto it = util::enums::inventoryTableColumns.begin(); it != util::enums::inventoryTableColumns.end(); it++)
    {
        this->schema.push_back(it->second);
    }
}

InventoryTable::~InventoryTable()
{
    InventoryTable::instance = NULL;
}

InventoryTable *InventoryTable::getInstance()
{
    if (InventoryTable::instance == NULL)
    {
        InventoryTable::instance = new InventoryTable(util::enums::tableNamesMap[util::enums::TableNames::INVENTORY],
                                                      util::enums::sequenceNamesMap[util::enums::SequenceNames::INVENTORY]);
    }
    return InventoryTable::instance;
}

// purchase transaction table
PurchaseTransactionTable::PurchaseTransactionTable(std::string tableName, std::string sequenceName) : Table::Table(tableName, sequenceName)
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
        PurchaseTransactionTable::instance = new PurchaseTransactionTable(util::enums::tableNamesMap[util::enums::TableNames::PURCHASETRANSACTION],
                                                                          util::enums::sequenceNamesMap[util::enums::SequenceNames::PURCHASETRANSACTION]);
    }
    return PurchaseTransactionTable::instance;
}

// purchase entry table
PurchaseEntryTable::PurchaseEntryTable(std::string tableName, std::string sequenceName) : Table::Table(tableName, sequenceName)
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
        PurchaseEntryTable::instance = new PurchaseEntryTable(util::enums::tableNamesMap[util::enums::TableNames::PURCHASEENTRY],
                                                              util::enums::sequenceNamesMap[util::enums::SequenceNames::PURCHASEENTRY]);
    }
    return PurchaseEntryTable::instance;
}

// selling entry table
SellingEntryTable::SellingEntryTable(std::string tableName, std::string sequenceName) : Table::Table(tableName, sequenceName)
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
        SellingEntryTable::instance = new SellingEntryTable(util::enums::tableNamesMap[util::enums::TableNames::SELLINGENTRY],
                                                            util::enums::sequenceNamesMap[util::enums::SequenceNames::SELLINGENTRY]);
    }
    return SellingEntryTable::instance;
}

// selling transaction table
SellingTransactionTable::SellingTransactionTable(std::string tableName, std::string sequenceName) : Table::Table(tableName, sequenceName)
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
        SellingTransactionTable::instance = new SellingTransactionTable(util::enums::tableNamesMap[util::enums::TableNames::SELLINGTRANSACTION],
                                                                        util::enums::sequenceNamesMap[util::enums::SequenceNames::SELLINGTRANSACTION]);
    }
    return SellingTransactionTable::instance;
}

// Equipment Table
AssetsTable::AssetsTable(std::string tableName, std::string sequenceName) : Table::Table(tableName, sequenceName)
{
    for (auto it = util::enums::assetsTableColumns.begin(); it != util::enums::assetsTableColumns.end(); it++)
    {
        this->schema.push_back(it->second);
    }
}

AssetsTable::~AssetsTable()
{
    AssetsTable::instance = NULL;
}

AssetsTable *AssetsTable::getInstance()
{
    if (AssetsTable::instance == NULL)
    {
        AssetsTable::instance = new AssetsTable(util::enums::tableNamesMap[util::enums::TableNames::ASSETS], util::enums::sequenceNamesMap[util::enums::SequenceNames::ASSETS]);
    }
    return AssetsTable::instance;
}

// accounting transaction table
AccountingTransactionTable::AccountingTransactionTable(std::string tableName, std::string sequenceName) : Table::Table(tableName, sequenceName)
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
        AccountingTransactionTable::instance = new AccountingTransactionTable(util::enums::tableNamesMap[util::enums::TableNames::ACCOUNTINGTRANSACTION],
                                                                              util::enums::sequenceNamesMap[util::enums::SequenceNames::ACCOUNTINGTRANSACTION]);
    }
    return AccountingTransactionTable::instance;
}

// accounting entry table
AccountingEntryTable::AccountingEntryTable(std::string tableName, std::string sequenceName) : Table::Table(tableName, sequenceName)
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
        AccountingEntryTable::instance = new AccountingEntryTable(util::enums::tableNamesMap[util::enums::TableNames::ACCOUNTINGENTRY],
                                                                  util::enums::sequenceNamesMap[util::enums::SequenceNames::ACCOUNTINGENTRY]);
    }
    return AccountingEntryTable::instance;
}