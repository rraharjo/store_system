#include "inventory/assets/items.hpp"

using namespace inventory;

Item::Item(util::enums::PrimaryKeyPrefix primary_key_prefix, std::string name = "", std::string item_code = "")
    : HasTable(primary_key_prefix)
{
    this->name = name;
    this->item_code = item_code;
    this->purchase_history = std::make_unique<PurchaseHistory>();
    this->selling_history = std::make_unique<SellingHistory>();
}

Item::~Item()
{
}

void Item::add_existing_purchase_entry(PurchaseEntry *entry)
{
    std::shared_ptr<Entry> to_add;
    to_add.reset(entry);
    this->purchase_history->add_entry(to_add);
}

void Item::add_existing_selling_entry(SellingEntry *entry)
{
    std::shared_ptr<Entry> to_add;
    to_add.reset(entry);
    this->selling_history->add_entry(to_add);
}

std::vector<std::shared_ptr<PurchaseEntry>> Item::get_purchase_entries()
{
    std::vector<std::shared_ptr<PurchaseEntry>> to_ret;
    for (std::shared_ptr<Entry> entry : this->purchase_history->get_entries())
    {
        std::shared_ptr<PurchaseEntry> temp = std::static_pointer_cast<PurchaseEntry>(entry);
        to_ret.push_back(temp);
    }
    return to_ret;
}

std::vector<std::shared_ptr<SellingEntry>> Item::get_selling_entries()
{
    std::vector<std::shared_ptr<SellingEntry>> to_ret;
    for (std::shared_ptr<Entry> entry : this->selling_history->get_entries())
    {
        std::shared_ptr<SellingEntry> temp = std::static_pointer_cast<SellingEntry>(entry);
        to_ret.push_back(temp);
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
