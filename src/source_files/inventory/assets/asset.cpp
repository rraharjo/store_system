#include "inventory/assets/asset.hpp"
using namespace inventory;

util::Table *Asset::class_table = util::AssetsTable::get_instance();

void Asset::insert_to_db()
{
    this->insert_to_db_with_table(Asset::class_table);
};

void Asset::update_to_db()
{
    this->update_to_db_with_table(Asset::class_table);
};

Asset::Asset(std::string db_code, std::string name, std::string item_code,
             double total_value, double residual_value, int year_useful_life,
             util::Date *date_bought, util::Date *last_depreciation_date, util::Date *date_sold)
    : Item(name, item_code)
{
    this->set_db_code(db_code);
    this->name = name;
    this->value = total_value;
    this->residual_value = residual_value;
    this->year_useful_life = year_useful_life;
    this->date_bought = date_bought;
    this->last_depreciation_date = last_depreciation_date;
    this->expiry_date = date_sold;
}

Asset::Asset(std::string name, std::string item_code, double residual_value, int year_useful_life, util::Date *date_bought)
    : Asset("", name, item_code, 0, residual_value, year_useful_life, date_bought, NULL, NULL)
{
}

void Asset::add_existing_purchase_entry(PurchaseEntry *entry)
{
    Item::add_existing_purchase_entry(entry);
}

std::vector<std::string> Asset::get_update_parameter()
{
    std::vector<std::string> args;
    args.push_back(this->name);
    args.push_back(std::to_string(this->get_total_value()));
    args.push_back(std::to_string(this->get_residual_value()));
    args.push_back(std::to_string(this->get_year_useful_life()));
    args.push_back(this->get_date_bought()->to_db_format());
    args.push_back(this->get_last_depreciation_date() ? this->get_last_depreciation_date()->to_db_format() : "NULL");
    args.push_back(this->get_expiry_date() ? this->get_expiry_date()->to_db_format() : "NULL");
    return args;
};

double Asset::sell_items(SellingEntry *entry)
{
    this->selling_history->add_entry(entry);
    this->expiry_date = entry->get_transaction_date();
    this->update_to_db();
    return this->get_total_value();
}

void Asset::add_purchase(PurchaseEntry *entry)
{
    set_total_value(this->value + entry->get_price());
    this->purchase_history->add_entry(entry);
    this->update_to_db();
}

double Asset::get_total_value()
{
    return this->value;
}

double Asset::get_residual_value()
{
    return this->residual_value;
}

int Asset::get_year_useful_life()
{
    return this->year_useful_life;
}

util::Date *Asset::get_date_bought()
{
    return this->date_bought;
}

util::Date *Asset::get_last_depreciation_date()
{
    return this->last_depreciation_date;
}

util::Date *Asset::get_expiry_date()
{
    return this->expiry_date;
}

double Asset::get_current_value()
{
    return this->get_total_value();
}

void Asset::set_total_value(double new_value)
{
    this->value = new_value;
}

void Asset::set_last_depreciation_date(util::Date *depreciation_date)
{
    this->last_depreciation_date = depreciation_date;
    this->update_to_db();
}

std::string Asset::to_string()
{
    std::string to_ret = "";
    to_ret += "DB code " + this->get_db_code() + "\n";
    to_ret += "name : " + this->name + "\n";
    to_ret += "valuation: " + std::to_string(this->value) + "\n";
    to_ret += "purchase date: " + this->get_date_bought()->to_string() + "\n";
    to_ret += "sold date: ";
    to_ret += this->get_expiry_date() ? this->get_expiry_date()->to_string() : "not sold";
    to_ret += "\n";
    return to_ret;
}