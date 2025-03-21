#include "inventory/assets/inventory.hpp"

using namespace inventory;

Inventory::Inventory(std::string db_code, std::string item_code, std::string name, double selling_price)
    : Item::Item(util::enums::PrimaryKeyPrefix::INVENTORY, name, item_code)
{
    this->set_db_code(db_code);
    this->qty = 0;
    this->selling_price = selling_price;
}

Inventory::Inventory(std::string item_code, std::string name, double selling_price)
    : Inventory("", item_code, name, selling_price)
{
}

void Inventory::add_existing_purchase_entry(PurchaseEntry *entry)
{
    Item::add_existing_purchase_entry(entry);
    this->qty += entry->get_available_qty();
}

double Inventory::sell_items(std::shared_ptr<SellingEntry> entry)
{
    if (this->qty < entry->get_qty())
    {
        throw std::invalid_argument("Purchasing quantity exceeds available quantity");
        return -1;
    }
    this->selling_history->add_entry(entry);
    this->qty -= entry->get_qty();
    return this->purchase_history->sell_item_first_in(entry->get_qty());
}

void Inventory::add_purchase(std::shared_ptr<PurchaseEntry> entry)
{
    this->purchase_history->add_entry(entry);
    this->qty += entry->get_available_qty();
}

double Inventory::get_selling_price()
{
    return this->selling_price;
}

void Inventory::set_selling_price(double new_price)
{
    this->selling_price = new_price;
}

std::string Inventory::to_string()
{
    std::string to_ret = "";
    to_ret += "DB code: " + this->get_db_code() + "\n";
    to_ret += "item code: " + this->item_code + "\n";
    to_ret += "name: " + this->name + "\n";
    to_ret += "selling price: " + std::to_string(this->selling_price) + "\n";
    to_ret += "qty: " + std::to_string(this->qty) + "\n";
    return to_ret;
}