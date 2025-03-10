#include "util/database/tables.hpp"
using namespace util;
std::string generate_column_name(std::vector<std::string> &names)
{
    std::string result = "";
    for (int i = 0; i < names.size() - 1; i++)
    {
        result += names[i] + ", ";
    }
    result += names[names.size() - 1];
    return result;
}

std::string generate_column_name(std::string abbreviation, std::vector<std::string> &names)
{
    std::string result = "";
    for (int i = 0; i < names.size() - 1; i++)
    {
        result += abbreviation + "." + names[i] + ", ";
    }
    result += names[names.size() - 1];
    return result;
}

std::string generate_single_condition(util::TableCondition &condition)
{
    util::TableComparator comp = condition.comparator;
    util::enums::ColumnTypes type = condition.col.type;
    std::string result = condition.col.column_name;

    switch (comp)
    {
    case TableComparator::LESSTHAN:
        result += " < ";
        break;
    case TableComparator::LESSTHANEQUAL:
        result += " <= ";
        break;
    case TableComparator::EQUAL:
        result += " = ";
        break;
    case TableComparator::MORETHANEQUAL:
        result += " >= ";
        break;
    case TableComparator::MORETHAN:
        result += " > ";
        break;
    case TableComparator::LIKE:
        result += " like ";
        break;
    case TableComparator::IS:
        result += " is ";
        break;
    case TableComparator::ISNOT:
        result += " is not ";
        break;
    default:
        break;
    }
    if (condition.value == "null")
    {
        result += "null";
        return result;
    }
    switch (type)
    {
    case util::enums::ColumnTypes::BOOLCOL:
    case util::enums::ColumnTypes::NUMBERCOL:
    case util::enums::ColumnTypes::FLOATCOL:
        result += condition.value;
        break;
    case util::enums::ColumnTypes::CUSTOMSERIALCOL:
    case util::enums::ColumnTypes::TEXTCOL:
        result += "'" + condition.value + "'";
        break;
    case util::enums::ColumnTypes::DATECOL:
        result += "to_date('" + condition.value + "', 'dd-mm-yyyy')";
    default:
        break;
    }
    // result += " ";
    return result;
}

std::string generate_conditions(std::vector<util::TableCondition> &conditions)
{
    std::string query = "";
    for (int i = 0; i < conditions.size() - 1; i++)
    {
        query += generate_single_condition(conditions[i]);
        query += " and ";
    }
    query += generate_single_condition(conditions[conditions.size() - 1]);
    return query;
}

InventoryTable *InventoryTable::instance = NULL;
PurchaseTransactionTable *PurchaseTransactionTable::instance = NULL;
PurchaseEntryTable *PurchaseEntryTable::instance = NULL;
SellingEntryTable *SellingEntryTable::instance = NULL;
SellingTransactionTable *SellingTransactionTable::instance = NULL;
AccountingTransactionTable *AccountingTransactionTable::instance = NULL;
AccountingEntryTable *AccountingEntryTable::instance = NULL;
AssetsTable *AssetsTable::instance = NULL;
TAccountTable *TAccountTable::instance = NULL;

// parent class
Table::Table(std::string table_name, std::string sequence_name)
{
    this->table_name = table_name;
    this->sequence_name = sequence_name;
}
std::string Table::get_table_name()
{
    return this->table_name;
}

std::vector<util::ColumnSchema> &Table::get_schema()
{
    return this->schema;
}

ColumnSchema Table::get_column(std::string col_name)
{
    for (ColumnSchema schema : this->schema)
    {
        if (schema.column_name == col_name)
        {
            return schema;
        }
    }
    return ColumnSchema();
}

std::vector<std::vector<std::string>> Table::get_records(
    std::vector<std::string> columns,
    std::vector<TableCondition> conditions)
{
    size_t col_size = columns.size(), cond_size = conditions.size();
    std::string query = "select " + generate_column_name(columns) + " from " + this->table_name + " where ";
    query += generate_conditions(conditions);
    query += ";";
    DB *instance = DB::get_instance();
    return instance->execute_query(query);
}

std::vector<std::vector<std::string>> Table::get_records(std::vector<TableCondition> conditions)
{
    size_t size = conditions.size();
    std::vector<std::string> col_names;
    if (size == 0)
    {
        return this->get_records();
    }
    for (ColumnSchema schema : this->get_schema())
    {
        col_names.push_back(schema.column_name);
    }
    std::string query = "select " + generate_column_name(col_names) + " from " + this->table_name + " where ";
    query += generate_conditions(conditions);
    query += ";";
    DB *instance = DB::get_instance();
    return instance->execute_query(query);
}

std::vector<std::vector<std::string>> Table::get_records()
{
    std::vector<std::string> col_names;
    for (ColumnSchema schema : this->get_schema())
    {
        col_names.push_back(schema.column_name);
    }
    std::string query = "select " + generate_column_name(col_names) + " from " + this->table_name + ";";
    DB *instance = DB::get_instance();
    return instance->execute_query(query);
}

std::vector<std::string> Table::insert_row(std::vector<std::string> &values)
{
    int schema_size = this->get_schema().size();
    int values_size = values.size();
    std::string query = "insert into " + this->get_table_name() + "(";
    for (int i = 0; i < schema_size - 1; i++)
    {
        query += this->get_schema()[i].column_name + ", ";
    }
    query += this->get_schema()[schema_size - 1].column_name + ") values(";
    int cur_value = 0;
    for (int i = 0; i < schema_size - 1; i++)
    {
        if (values[cur_value] == "NULL")
        {
            query += "NULL,";
            cur_value++;
            continue;
        }
        switch (this->get_schema()[i].type)
        {
        case util::enums::ColumnTypes::CUSTOMSERIALCOL:
            query += "concat('" + values[cur_value++] +
                     "', lpad(nextval('" + this->sequence_name + "')::text, 6, '0')), ";
            break;
        case util::enums::ColumnTypes::TEXTCOL:
            query += "'" + values[cur_value++] + "',";
            break;
        case util::enums::ColumnTypes::DATECOL:
            query += "to_date('" + values[cur_value++] + "', 'dd-MM-yyyy'),";
            break;
        case util::enums::ColumnTypes::BOOLCOL:
            query += values[cur_value++] + ",";
            break;
        case util::enums::ColumnTypes::FLOATCOL:
        case util::enums::ColumnTypes::NUMBERCOL:
            query += values[cur_value++] + ",";
            break;
        }
    }
    if (values[cur_value] == "NULL")
    {
        query += "NULL)";
    }
    else
    {
        switch (this->get_schema()[schema_size - 1].type)
        {
        case util::enums::ColumnTypes::CUSTOMSERIALCOL:
            query += "concat('" + values[cur_value++] +
                     "', lpad(nextval('" + this->sequence_name + "')::text, 6, '0'))";
            break;
        case util::enums::ColumnTypes::TEXTCOL:
            query += "'" + values[cur_value++] + "')";
            break;
        case util::enums::ColumnTypes::DATECOL:
            query += "to_date('" + values[cur_value++] + "', 'dd-MM-yyyy'))";
            break;
        case util::enums::ColumnTypes::BOOLCOL:
            query += values[cur_value++] + ")"; // Error
            break;
        case util::enums::ColumnTypes::FLOATCOL:
        case util::enums::ColumnTypes::NUMBERCOL:
            query += values[cur_value++] + ")";
            break;
        }
    }

    query += "returning *;";
    DB *instance = DB::get_instance();
    return instance->execute_query(query)[0];
}

std::vector<std::string> Table::update_row(std::string id, std::vector<std::string> &values)
{
    int schema_size = this->get_schema().size();
    std::string query = "update " + this->table_name + " set ";
    int cur_value = 0;
    for (int i = 0; i < schema_size - 1; i++)
    {
        util::ColumnSchema current_col = this->get_schema()[i];
        if (current_col.type == util::enums::ColumnTypes::CUSTOMSERIALCOL)
        {
            continue;
        }
        if (values[cur_value] == "NULL")
        {
            query += current_col.column_name + " = NULL,";
            cur_value++;
            continue;
        }

        switch (current_col.type)
        {
        case util::enums::ColumnTypes::TEXTCOL:
            query += current_col.column_name + " = ";
            query += "'" + values[cur_value++] + "',";
            break;
        case util::enums::ColumnTypes::DATECOL:
            query += current_col.column_name + " = ";
            query += "to_date('" + values[cur_value++] + "', 'dd-MM-yyyy'),";
            break;
        case util::enums::ColumnTypes::BOOLCOL:
            query += current_col.column_name + " = ";
            query += std::stoi(values[cur_value++]) ? "true," : "false,";
            break;
        case util::enums::ColumnTypes::FLOATCOL:
        case util::enums::ColumnTypes::NUMBERCOL:
            query += current_col.column_name + " = ";
            query += values[cur_value++] + ",";
            break;
        }
    }
    util::ColumnSchema current_col = this->get_schema()[schema_size - 1];
    if (current_col.type == util::enums::ColumnTypes::CUSTOMSERIALCOL)
    {
        query[query.length() - 1] = ' ';
    }
    else if (values[cur_value] == "NULL")
    {
        query += current_col.column_name + " = NULL ";
        cur_value++;
    }
    else
    {
        switch (current_col.type)
        {
        case util::enums::ColumnTypes::TEXTCOL:
            query += current_col.column_name + " = ";
            query += "'" + values[cur_value++] + "' ";
            break;
        case util::enums::ColumnTypes::DATECOL:
            query += current_col.column_name + " = ";
            query += "to_date('" + values[cur_value++] + "', 'dd-MM-yyyy') ";
            break;
        case util::enums::ColumnTypes::BOOLCOL:
            query += current_col.column_name + " = ";
            query += std::stoi(values[cur_value++]) ? "true " : "false ";
            break;
        case util::enums::ColumnTypes::FLOATCOL:
        case util::enums::ColumnTypes::NUMBERCOL:
            query += current_col.column_name + " = ";
            query += values[cur_value++] + " ";
            break;
        }
    }
    query += "where " + this->get_schema()[0].column_name + " = '" + id + "' returning *;";
    DB *instance = DB::get_instance();
    return instance->execute_query(query)[0];
}

// Inventory Table
InventoryTable::InventoryTable(std::string table_name, std::string sequence_name)
    : Table::Table(table_name, sequence_name)
{
    for (auto it = util::enums::inventory_table_columns.begin(); it != util::enums::inventory_table_columns.end(); it++)
    {
        this->schema.push_back(it->second);
    }
}

InventoryTable::~InventoryTable()
{
    InventoryTable::instance = NULL;
}

InventoryTable *InventoryTable::get_instance()
{
    if (InventoryTable::instance == NULL)
    {
        InventoryTable::instance = new InventoryTable(util::enums::table_names_map[util::enums::TableNames::INVENTORY],
                                                      util::enums::sequence_names_map[util::enums::SequenceNames::INVENTORY]);
    }
    return InventoryTable::instance;
}

// purchase transaction table
PurchaseTransactionTable::PurchaseTransactionTable(std::string table_name, std::string sequence_name)
    : Table::Table(table_name, sequence_name)
{
    for (auto it = util::enums::purchase_transaction_table_columns.begin(); it != util::enums::purchase_transaction_table_columns.end(); it++)
    {
        this->schema.push_back(it->second);
    }
}

PurchaseTransactionTable::~PurchaseTransactionTable()
{
    PurchaseTransactionTable::instance = NULL;
}

PurchaseTransactionTable *PurchaseTransactionTable::get_instance()
{
    if (PurchaseTransactionTable::instance == NULL)
    {
        PurchaseTransactionTable::instance =
            new PurchaseTransactionTable(util::enums::table_names_map[util::enums::TableNames::PURCHASETRANSACTION],
                                         util::enums::sequence_names_map[util::enums::SequenceNames::PURCHASETRANSACTION]);
    }
    return PurchaseTransactionTable::instance;
}

// purchase entry table
std::vector<std::vector<std::string>> PurchaseEntryTable::get_records(std::vector<std::string> columns,
                                                                      std::vector<TableCondition> conditions)
{
    std::string entry_abbr = "et", transaction_abbr = "pt";
    std::string transaction_table_name = util::enums::table_names_map[util::enums::TableNames::PURCHASETRANSACTION];
    std::string date_col =
        util::enums::purchase_transaction_table_columns[util::enums::PurchaseTransactionTable::DATEPURCHASED].column_name;
    std::string entry_join_col =
        util::enums::purchase_entry_table_columns[util::enums::PurchaseEntryTable::PURCHASETRANSACTIONCODE].column_name;
    std::string transaction_join_col =
        util::enums::purchase_transaction_table_columns[util::enums::PurchaseTransactionTable::DATABASECODE].column_name;
    size_t col_size = columns.size(), cond_size = conditions.size();
    std::string query = "select " + generate_column_name(entry_abbr, columns) + " from " +
                        this->get_table_name() + " " + entry_abbr + " inner join " + transaction_table_name + " " + transaction_abbr +
                        " on " + entry_abbr + "." + entry_join_col + " = " + transaction_abbr + "." + transaction_join_col + " where ";
    query += generate_conditions(conditions);
    query += " order by " + transaction_abbr + "." + date_col;
    query += ";";
    DB *instance = DB::get_instance();
    return instance->execute_query(query);
}

PurchaseEntryTable::PurchaseEntryTable(std::string table_name, std::string sequence_name)
    : Table::Table(table_name, sequence_name)
{
    for (auto it = util::enums::purchase_entry_table_columns.begin();
         it != util::enums::purchase_entry_table_columns.end(); it++)
    {
        this->schema.push_back(it->second);
    }
}

PurchaseEntryTable::~PurchaseEntryTable()
{
    PurchaseEntryTable::instance = NULL;
}

PurchaseEntryTable *PurchaseEntryTable::get_instance()
{
    if (PurchaseEntryTable::instance == NULL)
    {
        PurchaseEntryTable::instance =
            new PurchaseEntryTable(util::enums::table_names_map[util::enums::TableNames::PURCHASEENTRY],
                                   util::enums::sequence_names_map[util::enums::SequenceNames::PURCHASEENTRY]);
    }
    return PurchaseEntryTable::instance;
}

// selling entry table
SellingEntryTable::SellingEntryTable(std::string table_name, std::string sequence_name)
    : Table::Table(table_name, sequence_name)
{
    for (auto it = util::enums::selling_entry_table_columns.begin(); it != util::enums::selling_entry_table_columns.end(); it++)
    {
        this->schema.push_back(it->second);
    }
}

SellingEntryTable::~SellingEntryTable()
{
    SellingEntryTable::instance = NULL;
}

SellingEntryTable *SellingEntryTable::get_instance()
{
    if (SellingEntryTable::instance == NULL)
    {
        SellingEntryTable::instance =
            new SellingEntryTable(util::enums::table_names_map[util::enums::TableNames::SELLINGENTRY],
                                  util::enums::sequence_names_map[util::enums::SequenceNames::SELLINGENTRY]);
    }
    return SellingEntryTable::instance;
}

// selling transaction table
SellingTransactionTable::SellingTransactionTable(std::string table_name, std::string sequence_name)
    : Table::Table(table_name, sequence_name)
{
    for (auto it = util::enums::selling_transaction_table_columns.begin(); it != util::enums::selling_transaction_table_columns.end(); it++)
    {
        this->schema.push_back(it->second);
    }
}

SellingTransactionTable::~SellingTransactionTable()
{
    SellingTransactionTable::instance = NULL;
}

SellingTransactionTable *SellingTransactionTable::get_instance()
{
    if (SellingTransactionTable::instance == NULL)
    {
        SellingTransactionTable::instance =
            new SellingTransactionTable(util::enums::table_names_map[util::enums::TableNames::SELLINGTRANSACTION],
                                        util::enums::sequence_names_map[util::enums::SequenceNames::SELLINGTRANSACTION]);
    }
    return SellingTransactionTable::instance;
}

// Equipment Table
AssetsTable::AssetsTable(std::string table_name, std::string sequence_name) : Table::Table(table_name, sequence_name)
{
    for (auto it = util::enums::assets_table_columns.begin(); it != util::enums::assets_table_columns.end(); it++)
    {
        this->schema.push_back(it->second);
    }
}

AssetsTable::~AssetsTable()
{
    AssetsTable::instance = NULL;
}

AssetsTable *AssetsTable::get_instance()
{
    if (AssetsTable::instance == NULL)
    {
        AssetsTable::instance = new AssetsTable(util::enums::table_names_map[util::enums::TableNames::ASSETS],
                                                util::enums::sequence_names_map[util::enums::SequenceNames::ASSETS]);
    }
    return AssetsTable::instance;
}

// accounting transaction table
AccountingTransactionTable::AccountingTransactionTable(std::string table_name, std::string sequence_name)
    : Table::Table(table_name, sequence_name)
{
    for (auto it = util::enums::accounting_transaction_table_columns.begin();
         it != util::enums::accounting_transaction_table_columns.end(); it++)
    {
        this->schema.push_back(it->second);
    }
}

AccountingTransactionTable::~AccountingTransactionTable()
{
    AccountingTransactionTable::instance = NULL;
}

AccountingTransactionTable *AccountingTransactionTable::get_instance()
{
    if (AccountingTransactionTable::instance == NULL)
    {
        AccountingTransactionTable::instance =
            new AccountingTransactionTable(util::enums::table_names_map[util::enums::TableNames::ACCOUNTINGTRANSACTION],
                                           util::enums::sequence_names_map[util::enums::SequenceNames::ACCOUNTINGTRANSACTION]);
    }
    return AccountingTransactionTable::instance;
}

// accounting entry table
AccountingEntryTable::AccountingEntryTable(std::string table_name, std::string sequence_name)
    : Table::Table(table_name, sequence_name)
{
    for (auto it = util::enums::accounting_entry_table_columns.begin();
         it != util::enums::accounting_entry_table_columns.end(); it++)
    {
        this->schema.push_back(it->second);
    }
}

AccountingEntryTable::~AccountingEntryTable()
{
    AccountingEntryTable::instance = NULL;
}

AccountingEntryTable *AccountingEntryTable::get_instance()
{
    if (AccountingEntryTable::instance == NULL)
    {
        AccountingEntryTable::instance =
            new AccountingEntryTable(util::enums::table_names_map[util::enums::TableNames::ACCOUNTINGENTRY],
                                     util::enums::sequence_names_map[util::enums::SequenceNames::ACCOUNTINGENTRY]);
    }
    return AccountingEntryTable::instance;
}

// TAccount table
std::vector<std::string> TAccountTable::insert_row(std::vector<std::string> &values)
{
    int schema_size = this->get_schema().size();
    int values_size = values.size();
    std::string query = "insert into " + this->get_table_name() + "(";
    for (int i = 0; i < schema_size - 1; i++)
    {
        query += this->get_schema()[i].column_name + ", ";
    }
    query += this->get_schema()[schema_size - 1].column_name + ") values(";
    int cur_value = 0;
    for (int i = 0; i < schema_size - 1; i++)
    {
        if (values[cur_value] == "NULL")
        {
            query += "NULL,";
            cur_value++;
            continue;
        }
        switch (this->get_schema()[i].type)
        {
        case util::enums::ColumnTypes::CUSTOMSERIALCOL:
            query += "concat('" + values[cur_value++] +
                     "', lpad(nextval('" + this->sequence_name + "')::text, 6, '0')), ";
            break;
        case util::enums::ColumnTypes::TEXTCOL:
            query += "'" + values[cur_value++] + "',";
            break;
        case util::enums::ColumnTypes::DATECOL:
            query += "to_date('" + values[cur_value++] + "', 'dd-MM-yyyy'),";
            break;
        case util::enums::ColumnTypes::BOOLCOL:
            query += values[cur_value++] + ",";
            break;
        case util::enums::ColumnTypes::FLOATCOL:
        case util::enums::ColumnTypes::NUMBERCOL:
            query += values[cur_value++] + ",";
            break;
        }
    }
    if (values[cur_value] == "NULL")
    {
        query += "NULL)";
    }
    else
    {
        switch (this->get_schema()[schema_size - 1].type)
        {
        case util::enums::ColumnTypes::CUSTOMSERIALCOL:
            query += "concat('" + values[cur_value++] +
                     "', lpad(nextval('" + this->sequence_name + "')::text, 6, '0'))";
            break;
        case util::enums::ColumnTypes::TEXTCOL:
            query += "'" + values[cur_value++] + "')";
            break;
        case util::enums::ColumnTypes::DATECOL:
            query += "to_date('" + values[cur_value++] + "', 'dd-MM-yyyy'))";
            break;
        case util::enums::ColumnTypes::BOOLCOL:
            query += values[cur_value++] + ")"; // Error
            break;
        case util::enums::ColumnTypes::FLOATCOL:
        case util::enums::ColumnTypes::NUMBERCOL:
            query += values[cur_value++] + ")";
            break;
        }
    }

    query += " on conflict do nothing returning *;";
    DB *instance = DB::get_instance();
    std::vector<std::vector<std::string>> result = instance->execute_query(query);
    return result.size() ? result[0] : std::vector<std::string>();
}

TAccountTable::TAccountTable(std::string table_name)
    : Table::Table(table_name, "")
{
    for (auto it = util::enums::t_account_table_columns.begin(); it != util::enums::t_account_table_columns.end(); it++)
    {
        this->schema.push_back(it->second);
    }
}

TAccountTable::~TAccountTable()
{
    TAccountTable::instance = NULL;
}

TAccountTable *TAccountTable::get_instance()
{
    if (TAccountTable::instance == NULL)
    {
        TAccountTable::instance = new TAccountTable(util::enums::table_names_map[util::enums::TableNames::TACCOUNTS]);
    }
    return TAccountTable::instance;
}