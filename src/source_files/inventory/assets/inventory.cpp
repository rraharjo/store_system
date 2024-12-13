#include "inventory/assets/inventory.hpp"

using namespace inventory;

util::Table *Inventory::class_table = util::InventoryTable::get_instance();

std::vector<Inventory *> Inventory::generate_from_database()
{
    std::vector<Inventory *> to_ret;
    std::vector<std::vector<std::string>> records = Inventory::class_table->get_records();
    for (std::vector<std::string> &record : records)
    {
        Inventory *new_inventory = new Inventory(record[0], record[1], record[2], std::stod(record[3]));
        std::vector<PurchaseEntry *> entries = PurchaseEntry::generate_from_database(new_inventory->get_db_code());
        for (PurchaseEntry *entry : entries)
        {
            new_inventory->add_existing_purchase_entry(entry);
        }
        to_ret.push_back(new_inventory);
    }
    return to_ret;
}

void Inventory::insert_to_db()
{
    this->insert_to_db_with_table(Inventory::class_table);
};

void Inventory::update_to_db()
{
    this->update_to_db_with_table(Inventory::class_table);
};

Inventory::Inventory(std::string db_code, std::string item_code, std::string name, double selling_price)
    : Item::Item(name, item_code)
{
    this->set_db_code(db_code);
    this->qty = 0;
    this->selling_price = selling_price;
}

Inventory::Inventory(std::string item_code, std::string name, double selling_price)
    : Inventory("", item_code, name, selling_price)
{
}

std::vector<std::string> Inventory::get_insert_parameter()
{
    std::vector<std::string> args;
    args.push_back(util::enums::primary_key_codes_map[util::enums::PrimaryKeyCodes::INVENTORY]);
    args.push_back(this->get_item_code());
    args.push_back(this->get_name());
    args.push_back(std::to_string(this->get_selling_price()));
    return args;
};

std::vector<std::string> Inventory::get_update_parameter()
{
    std::vector<std::string> args;
    args.push_back(this->get_item_code());
    args.push_back(this->get_name());
    args.push_back(std::to_string(this->get_selling_price()));
    return args;
};

void Inventory::add_existing_purchase_entry(PurchaseEntry *entry)
{
    Item::add_existing_purchase_entry(entry);
    this->qty += entry->get_available_qty();
}

double Inventory::sell_items(SellingEntry *entry)
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

void Inventory::add_purchase(PurchaseEntry *entry)
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
    this->update_to_db();
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