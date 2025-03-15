#include "inventory/transaction/entry.hpp"
using namespace inventory;
Entry::Entry(util::enums::PrimaryKeyPrefix primary_key_prefix, std::string item_db_code, std::string transaction_code, double price, int qty)
    : util::baseclass::HasTable(primary_key_prefix)
{
    if (!item_db_code.compare(0, 3, util::enums::primary_key_prefix_map[util::enums::PrimaryKeyPrefix::INVENTORY]))
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

void Entry::set_item_db_code(std::string db_code)
{
    if (!db_code.compare(0, 3, util::enums::primary_key_prefix_map[util::enums::PrimaryKeyPrefix::INVENTORY]))
    {
        this->sellable_db_code = db_code;
        this->properties_db_code = "";
    }
    else
    {
        this->properties_db_code = db_code;
        this->sellable_db_code = "";
    }
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

void Entry::set_transaction_db_code(std::string db_code)
{
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

PurchaseEntry::PurchaseEntry(std::string db_code, std::string inventory_db_code, std::string transaction_db_code,
                             double price, int all_qty, int available_qty)
    : Entry(util::enums::PrimaryKeyPrefix::PURCHASEENTRY, inventory_db_code, transaction_db_code, price, all_qty)
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
int SellingEntry::next_item_code = 0;

SellingEntry::SellingEntry(std::string sellable_db_code, std::string transaction_code, double price, int qty)
    : Entry(util::enums::PrimaryKeyPrefix::SELLINGENTRY, sellable_db_code, transaction_code, price, qty)
{
}

SellingEntry::SellingEntry(std::string db_code, std::string sellable_db_code, std::string transaction_code, double price, int qty)
    : Entry(util::enums::PrimaryKeyPrefix::SELLINGENTRY, sellable_db_code, transaction_code, price, qty)
{
    this->set_db_code(db_code);
}