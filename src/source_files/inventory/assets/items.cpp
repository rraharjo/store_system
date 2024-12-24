#include "inventory/assets/items.hpp"

using namespace inventory;

Item::Item(std::string name = "", std::string item_code = "")
{
    this->name = name;
    this->item_code = item_code;
    this->purchase_history = new PurchaseHistory();
    this->selling_history = new SellingHistory();
}

void Item::add_existing_purchase_entry(PurchaseEntry *entry)
{
    this->purchase_history->add_entry(entry);
}

void Item::add_existing_selling_entry(SellingEntry *entry)
{
    this->selling_history->add_entry(entry);
}

std::string Item::get_name()
{
    return this->name;
}

std::string Item::get_item_code()
{
    return this->item_code;
}

int Item::get_qty()
{
    return this->qty;
}
