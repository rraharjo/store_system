#include "inventory/assets/equipment.hpp"
using namespace inventory;

std::vector<Asset *> Equipment::generate_from_database()
{
    std::vector<Asset *> to_ret;
    std::vector<util::TableCondition> conditions;
    util::TableCondition not_sold;
    not_sold.col = util::enums::assets_table_columns[util::enums::AssetsTable::DATESOLD];
    not_sold.comparator = util::TableComparator::IS;
    not_sold.value = "null";
    conditions.push_back(not_sold);
    std::vector<std::vector<std::string>> records = Asset::class_table->get_records(conditions);
    for (std::vector<std::string> &record : records)
    {
        util::Date *purchase = NULL, *depreciation_date = NULL, *sold = NULL;
        if (record[5] != "")
        {
            purchase = new util::Date(record[5], "%Y-%m-%d");
        }
        if (record[6] != ""){
            depreciation_date = new util::Date(record[6], "%Y-%m-%d");
        }
        if (record[7] != "")
        {
            sold = new util::Date(record[6], "%Y-%m-%d");
        }
        Asset *new_equipment = new Equipment(record[0], record[1], "", std::stod(record[2]),
                                            std::stod(record[3]), std::stoi(record[4]), purchase, depreciation_date, sold);
        std::vector<PurchaseEntry *> entries = PurchaseEntry::generate_from_database(new_equipment->get_db_code());
        for (PurchaseEntry *entry : entries)
        {
            new_equipment->add_existing_purchase_entry(entry);
        }
        to_ret.push_back(new_equipment);
    }
    return to_ret;
}

Equipment::Equipment(std::string db_code, std::string name, std::string item_code,
                     double total_value, double residual_value, int year_useful_life,
                     util::Date *date_bought, util::Date *last_depreciation_date, util::Date *date_sold)
    : Asset(db_code, name, item_code, total_value, residual_value, year_useful_life, date_bought, last_depreciation_date, date_sold)
{
    this->depreciation_method = new util::DoubleDecliningDepreciation(this->get_total_value(), this->get_year_useful_life());
}

Equipment::Equipment(std::string name, std::string item_code, double residual_value, int year_useful_life, util::Date *date_bought)
    : Equipment("", name, item_code, 0, residual_value, year_useful_life, date_bought, date_bought, NULL)
{
}

void Equipment::add_existing_purchase_entry(PurchaseEntry *entry)
{
    Asset::add_existing_purchase_entry(entry);
}

std::vector<std::string> Equipment::get_insert_parameter()
{
    std::vector<std::string> args;
    args.push_back(util::enums::primary_key_codes_map[util::enums::PrimaryKeyCodes::EQUIPMENT]);
    args.push_back(this->get_name());
    args.push_back(std::to_string(this->get_total_value()));
    args.push_back(std::to_string(this->get_residual_value()));
    args.push_back(std::to_string(this->get_year_useful_life()));
    args.push_back(this->get_date_bought()->to_db_format());
    args.push_back(this->get_last_depreciation_date() ? this->get_last_depreciation_date()->to_db_format() : "NULL");
    args.push_back(this->get_expiry_date() ? this->get_expiry_date()->to_db_format() : "NULL");
    return args;
};

double Equipment::get_reduced_value_at_year(int year)
{
    return this->get_depreciation_expense_at_year(year);
}

double Equipment::get_reduced_value_current_year()
{
    return this->get_current_depreciation_expense();
}

double Equipment::get_depreciation_expense_at_year(int year)
{
    if (year < 0 || year > this->year_useful_life)
    {
        return -1;
    }
    return this->depreciation_method->get_depreciation_expense_at_year(year);
}

double Equipment::get_accumulated_depreciation_at_year(int year)
{
    if (year < 0 || year > this->year_useful_life)
    {
        return -1;
    }
    return this->depreciation_method->get_accumulated_depreciation_at_year(year);
}

double Equipment::get_value_at_year(int year)
{
    if (year < 0 || year > this->year_useful_life)
    {
        return -1;
    }
    return this->get_total_value() - this->get_accumulated_depreciation_at_year(year);
}

// should be accounting ending period
double Equipment::get_current_depreciation_expense()
{
    util::Date *now = new util::Date();
    int age = this->date_bought->diff_years_to(now);
    return this->get_depreciation_expense_at_year(age);
}

double Equipment::get_current_accumulated_depreciation()
{
    util::Date *now = new util::Date();
    int age = this->date_bought->diff_years_to(now);
    return this->get_accumulated_depreciation_at_year(age);
}

double Equipment::get_current_value()
{
    util::Date *now = new util::Date();
    int age = this->date_bought->diff_years_to(now);
    delete now;
    return this->get_value_at_year(age);
}

void Equipment::set_total_value(double new_value)
{
    if (new_value < this->get_total_value())
    {
        return;
    }
    Asset::set_total_value(new_value);
    this->depreciation_method->set_cost(new_value);
}