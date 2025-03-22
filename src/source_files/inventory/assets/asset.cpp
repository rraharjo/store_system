#include "inventory/assets/asset.hpp"
using namespace inventory;

Asset::Asset(util::enums::PrimaryKeyPrefix primary_key_prefix,
             std::string db_code,
             std::string name,
             std::string item_code,
             double total_value,
             double residual_value,
             int year_useful_life,
             std::unique_ptr<util::Date> date_bought,
             std::unique_ptr<util::Date> last_depreciation_date,
             std::unique_ptr<util::Date> date_sold)
    : Item(primary_key_prefix, name, item_code)
{
    this->set_db_code(db_code);
    this->name = name;
    this->value = total_value;
    this->residual_value = residual_value;
    this->year_useful_life = year_useful_life;
    this->date_bought = std::move(date_bought);
    this->last_depreciation_date = std::move(last_depreciation_date);
    this->expiry_date = std::move(date_sold);
}

Asset::Asset(util::enums::PrimaryKeyPrefix primary_key_prefix,
             std::string name,
             std::string item_code,
             double residual_value,
             int year_useful_life,
             std::unique_ptr<util::Date> date_bought)
    : Asset(primary_key_prefix,
            "",
            name,
            item_code,
            0,
            residual_value,
            year_useful_life,
            std::move(date_bought),
            NULL,
            NULL)
{
}

Asset::~Asset()
{
#ifdef DEBUG
    std::cout << "Deleting Asset" << std::endl;
#endif
}

void Asset::add_existing_purchase_entry(std::unique_ptr<PurchaseEntry> entry)
{
    Item::add_existing_purchase_entry(std::move(entry));
}

double Asset::sell_items(std::shared_ptr<SellingEntry> entry)
{
    std::shared_ptr<Entry> to_add(entry);
    this->selling_history->add_entry(to_add);
    util::Date *new_date = new util::Date(*(entry->get_transaction_date()));
    this->expiry_date.reset(new_date);
    return this->get_total_value();
}

void Asset::add_purchase(std::shared_ptr<PurchaseEntry> entry)
{
    std::shared_ptr<Entry> to_add(entry);
    set_total_value(this->value + entry->get_price());
    this->purchase_history->add_entry(to_add);
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
    return this->date_bought.get();
}

util::Date *Asset::get_last_depreciation_date()
{
    return this->last_depreciation_date.get();
}

util::Date *Asset::get_expiry_date()
{
    return this->expiry_date.get();
}

double Asset::get_current_value()
{
    return this->get_total_value();
}

void Asset::set_total_value(double new_value)
{
    this->value = new_value;
}

void Asset::set_last_depreciation_date(std::unique_ptr<util::Date> new_date)
{
    this->last_depreciation_date.reset();
    this->last_depreciation_date = std::move(new_date);
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