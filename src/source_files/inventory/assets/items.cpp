#include "inventory/assets/items.hpp"

using namespace inventory;

Item::Item(util::enums::PrimaryKeyPrefix primary_key_prefix, std::string name = "", std::string item_code = "")
    : HasTable(primary_key_prefix)
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

std::vector<PurchaseEntry *> Item::get_purchase_entries()
{
    std::vector<PurchaseEntry *> to_ret;
    for (Entry *entry : this->purchase_history->get_entries())
    {
        to_ret.push_back((PurchaseEntry *)entry);
    }
    return to_ret;
}

std::vector<SellingEntry *> Item::get_selling_entries()
{
    std::vector<SellingEntry *> to_ret;
    for (Entry *entry : this->selling_history->get_entries())
    {
        to_ret.push_back((SellingEntry *)entry);
    }
    return to_ret;
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
