#include "inventory/transaction/entry.hpp"
using namespace inventory;
Entry::Entry(std::string item_db_code, std::string transaction_code, double price, int qty)
{
    if (!item_db_code.compare(0, 3, util::enums::primary_key_codes_map[util::enums::PrimaryKeyCodes::INVENTORY]))
    {
        this->sellable_db_code = item_db_code;
        this->properties_db_code = "";
    }
    else
    {
        this->properties_db_code = item_db_code;
        this->sellable_db_code = "";
    }
    this->transaction_db_code = transaction_code;
    this->price = price;
    this->qty = qty;
}

std::string Entry::get_sellable_db_code()
{
    return this->sellable_db_code;
}

std::string Entry::get_properties_db_code()
{
    return this->properties_db_code;
}

std::string Entry::get_transaction_db_code()
{
    return this->transaction_db_code;
}

double Entry::get_price()
{
    return this->price;
}

int Entry::get_qty()
{
    return this->qty;
}

util::Date *Entry::get_transaction_date()
{
    return this->transaction_date;
}

void Entry::set_transaction_date(util::Date *transaction_date)
{
    this->transaction_date = transaction_date;
}

void Entry::set_transaction_db_code(std::string db_code){
    this->transaction_db_code = db_code;
}

/******************************************************************************/

std::vector<util::TableCondition> get_inventory_condition(std::string inventory_db_code)
{
    std::vector<util::TableCondition> conditions;
    util::TableCondition cond1 = util::TableCondition(), cond2 = util::TableCondition();
    cond1.col = util::enums::purchase_entry_table_columns[util::enums::PurchaseEntryTable::INVENTORYDBCODE];
    cond1.comparator = util::TableComparator::EQUAL;
    cond1.value = inventory_db_code;
    cond2.col = util::enums::purchase_entry_table_columns[util::enums::PurchaseEntryTable::AVAILABLEQTY];
    cond2.comparator = util::TableComparator::MORETHAN;
    cond2.value = "0";
    conditions.push_back(cond1);
    conditions.push_back(cond2);
    return conditions;
}

std::vector<util::TableCondition> get_equipment_condition(std::string equipment_db_code)
{
    std::vector<util::TableCondition> conditions;
    util::TableCondition cond1 = util::TableCondition();
    cond1.col = util::enums::purchase_entry_table_columns[util::enums::PurchaseEntryTable::ASSETSCODE];
    cond1.comparator = util::TableComparator::EQUAL;
    cond1.value = equipment_db_code;
    conditions.push_back(cond1);
    return conditions;
}

util::Table *PurchaseEntry::class_table = util::PurchaseEntryTable::get_instance();

std::vector<PurchaseEntry *> PurchaseEntry::generate_from_database(std::string item_db_code)
{
    std::vector<PurchaseEntry *> to_ret;
    std::vector<util::TableCondition> conditions;
    std::vector<std::string> columns;
    std::vector<std::vector<std::string>> records;
    size_t item_index;
    for (auto it = util::enums::purchase_entry_table_columns.begin(); it != util::enums::purchase_entry_table_columns.end(); it++)
    {
        columns.push_back(it->second.column_name);
    }
    if (item_db_code.compare(0, 3, "INV") == 0){
        conditions = get_inventory_condition(item_db_code);
        item_index = 1;
    }
    else{
        conditions = get_equipment_condition(item_db_code);
        item_index = 2;
    }
    records = PurchaseEntry::class_table->get_records(columns, conditions);
    for (std::vector<std::string> &record : records)
    {
        PurchaseEntry *new_entry = new PurchaseEntry(record[0], record[item_index], record[3],
                                                    std::stod(record[4]), std::stoi(record[5]), std::stoi(record[6]));
        to_ret.push_back(new_entry);
    }
    return to_ret;
}

std::vector<std::string> PurchaseEntry::get_insert_parameter()
{
    std::vector<std::string> args;
    args.push_back(util::enums::primary_key_codes_map[util::enums::PrimaryKeyCodes::PURCHASEENTRY]);
    args.push_back(this->get_sellable_db_code() == "" ? "NULL" : this->get_sellable_db_code());
    args.push_back(this->get_properties_db_code() == "" ? "NULL" : this->get_properties_db_code());
    args.push_back(this->get_transaction_db_code());
    args.push_back(std::to_string(this->get_price()));
    args.push_back(std::to_string(this->get_qty()));
    args.push_back(std::to_string(this->get_available_qty()));
    return args;
}

std::vector<std::string> PurchaseEntry::get_update_parameter()
{
    std::vector<std::string> args;
    args.push_back(this->get_sellable_db_code() == "" ? "NULL" : this->get_sellable_db_code());
    args.push_back(this->get_properties_db_code() == "" ? "NULL" : this->get_properties_db_code());
    args.push_back(this->get_transaction_db_code());
    args.push_back(std::to_string(this->get_price()));
    args.push_back(std::to_string(this->get_qty()));
    args.push_back(std::to_string(this->get_available_qty()));
    return args;
}

void PurchaseEntry::insert_to_db()
{
    this->insert_to_db_with_table(PurchaseEntry::class_table);
}

void PurchaseEntry::update_to_db()
{
    this->update_to_db_with_table(PurchaseEntry::class_table);
}

PurchaseEntry::PurchaseEntry(std::string db_code, std::string inventory_db_code, std::string transaction_db_code,
                             double price, int all_qty, int available_qty)
    : Entry(inventory_db_code, transaction_db_code, price, all_qty)
{
    this->set_db_code(db_code);
    this->available_qty = available_qty;
}

PurchaseEntry::PurchaseEntry(std::string inventory_db_code, std::string transaction_db_code, double price, int qty)
    : PurchaseEntry("", inventory_db_code, transaction_db_code, price, qty, qty)
{
}

int PurchaseEntry::get_available_qty()
{
    return this->available_qty;
}
void PurchaseEntry::set_available_qty(int qty)
{
    this->available_qty = qty;
}

/************************************************************************/
util::Table *SellingEntry::class_table = util::SellingEntryTable::get_instance();
int SellingEntry::next_item_code = 0; 

std::vector<std::string> SellingEntry::get_insert_parameter()
{
    std::vector<std::string> args;
    args.push_back(util::enums::primary_key_codes_map[util::enums::PrimaryKeyCodes::SELLINGENTRY]);
    args.push_back(this->get_sellable_db_code() == "" ? "NULL" : this->get_sellable_db_code());
    args.push_back(this->get_properties_db_code() == "" ? "NULL" : this->get_properties_db_code());
    args.push_back(this->get_transaction_db_code());
    args.push_back(std::to_string(this->get_price()));
    args.push_back(std::to_string(this->get_qty()));
    return args;
}

std::vector<std::string> SellingEntry::get_update_parameter()
{
    std::vector<std::string> args;
    args.push_back(this->get_sellable_db_code() == "" ? "NULL" : this->get_sellable_db_code());
    args.push_back(this->get_properties_db_code() == "" ? "NULL" : this->get_properties_db_code());
    args.push_back(this->get_transaction_db_code());
    args.push_back(std::to_string(this->get_price()));
    args.push_back(std::to_string(this->get_qty()));
    return args;
}

void SellingEntry::insert_to_db()
{
    this->insert_to_db_with_table(SellingEntry::class_table);
}

void SellingEntry::update_to_db()
{
    this->update_to_db_with_table(SellingEntry::class_table);
}

SellingEntry::SellingEntry(std::string sellable_db_code, std::string transaction_code, double price, int qty) : Entry(sellable_db_code, transaction_code, price, qty)
{
}