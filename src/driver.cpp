#include <iostream>
#include <limits>
#include "store/store_system.hpp"

#define OPT_SIZE 9
std::string menu();
int get_int(std::string, int, int);
double get_double(std::string, double, double);
std::string get_string(std::string);
util::Date *get_date(std::string);
void add_new_inventory(store::StoreSystem *);
void purchase_inventory(store::StoreSystem *);
void add_purchase_entry(store::PurchaseTransaction *);
void purchase_asset(store::StoreSystem *);
void capitalize_assets(store::StoreSystem *);
void sell_inventory(store::StoreSystem *);
void sell_assets(store::StoreSystem *);
void end_of_year(store::StoreSystem *);
void list_of_items(store::StoreSystem *);

int main(int argc, char *argv[])
{
    store::StoreSystem *my_system = store::StoreSystem::get_instance();
    int option = -1;
    while (option)
    {
        std::cout << menu();
        option = get_int("Enter your choice:", 0, OPT_SIZE - 1);
        switch (option)
        {
        case 1:
            add_new_inventory(my_system);
            break;
        case 2:
            purchase_inventory(my_system);
            break;
        case 3:
            purchase_asset(my_system);
            break;
        case 4:
            capitalize_assets(my_system);
            break;
        case 5:
            sell_inventory(my_system);
            break;
        case 6:
            sell_assets(my_system);
            break;
        case 7:
            end_of_year(my_system);
            break;
        case 8:
            list_of_items(my_system);
            break;
        default:
            break;
        }
    }
}

std::string menu()
{
    std::string to_ret = "0. exit\n";
    to_ret += "1. Add new inventory\n";
    to_ret += "2. Purchase inventory\n";
    to_ret += "3. Purchase asset\n";
    to_ret += "4. Capitalize asset\n";
    to_ret += "5. Sell inventory\n";
    to_ret += "6. Sell Asset\n";
    to_ret += "7. End of the year\n";
    to_ret += "8. Get a list of item\n";
    return to_ret;
}

int get_int(std::string message, int int_min, int int_max)
{
    int option = -1;
    std::cout << message << '\n';
    std::cout << "Enter option between " << int_min << " and " << int_max << '\n';
    while (1)
    {
        std::cin >> option;
        if (option >= int_min && option <= int_max)
        {
            break;
        }
        std::cout << "Invalid option\n";
    }
    return option;
}

double get_double(std::string message, double d_min, double d_max)
{
    double option;
    std::cout << message << '\n';
    std::cout << "Enter double between " << d_min << " and " << d_max << '\n';
    while (1)
    {
        std::cin >> option;
        if (option >= d_min && option <= d_max)
        {
            break;
        }
        std::cout << "Invalid option\n";
    }
    return option;
}

std::string get_string(std::string message)
{
    std::string to_ret = "";
    std::cout << message << '\n';
    std::cin >> to_ret;
    return to_ret;
}

util::Date *get_date(std::string message)
{
    std::cout << "format: dd/MM/yyyy\n";
    std::string format = get_string(message);
    util::Date *to_ret = new util::Date(format);
    return to_ret;
}

void add_new_inventory(store::StoreSystem *s_system)
{
    std::string name = get_string("Enter product name:");
    std::string item_code = get_string("Enter item code:");
    double price = get_double("Enter price:", 0.0, std::numeric_limits<double>::max());
    inventory::Inventory *new_inventory = new inventory::Inventory(item_code, name, price);
    new_inventory->insert_to_db();
    s_system->add_item(new_inventory);
    std::cout << "new inventory added" << '\n';
}

void purchase_inventory(store::StoreSystem *s_system)
{
    std::string seller = get_string("Enter transaction seller:");
    util::Date *trans_date = get_date("Enter date: ");
    store::PurchaseTransaction *new_transaction = new store::PurchaseTransaction(seller, trans_date);
    add_purchase_entry(new_transaction);
    int more = get_int("more?", 0, 1);
    while (more)
    {
        add_purchase_entry(new_transaction);
        more = get_int("more?", 0, 1);
    }
    double paid_cash = get_double("How much cash paid? ", 0, new_transaction->get_transaction_amount());
    new_transaction->set_paid_cash(paid_cash);
    new_transaction->set_paid_credit(new_transaction->get_transaction_amount() - paid_cash);
    new_transaction->insert_to_db();
    s_system->buy_item(new_transaction);
    return;
}

void add_purchase_entry(store::PurchaseTransaction *p)
{
    std::string item_code = get_string("Enter item DB Code:");
    double price = get_double("Enter each item price", 0.0, std::numeric_limits<double>::max());
    int qty = get_int("Enter qty purchased: ", 0, std::numeric_limits<int>::max());
    inventory::PurchaseEntry *new_entry = new inventory::PurchaseEntry(item_code, p->get_db_code(), price, qty);
    p->add_entry(new_entry);
}

void purchase_asset(store::StoreSystem *s_system)
{
    std::string name = get_string("Name: ");
    std::string item_code = get_string("item code: ");
    double purchase_cost = get_double("Purchase cost: ", 0.0, std::numeric_limits<double>::max());
    double residual_value = get_double("Residual value: ", 0.0, std::numeric_limits<double>::max());
    int useful_life = get_int("Year useful life: ", 1, 99);
    util::Date *date_purchased = get_date("date purchased ");
    inventory::Equipment *new_eqp = new inventory::Equipment(name, item_code, residual_value, useful_life, date_purchased);
    new_eqp->insert_to_db();
    store::PurchaseTransaction *new_transaction = new store::PurchaseTransaction("", date_purchased);
    inventory::PurchaseEntry *new_entry = new inventory::PurchaseEntry(new_eqp->get_db_code(), "", purchase_cost, 1);
    new_transaction->add_entry(new_entry);
    double paid_cash = get_double("How much cash paid? ", 0, new_transaction->get_transaction_amount());
    new_transaction->set_paid_cash(paid_cash);
    new_transaction->set_paid_credit(new_transaction->get_transaction_amount() - paid_cash);
    new_transaction->insert_to_db();
    s_system->add_property(new_eqp);
    s_system->capitalize_asset(new_transaction);
}

void capitalize_assets(store::StoreSystem *s_system)
{
    std::string item_code = get_string("item code: ");
    double capitalized_amt = get_double("Capitalized amount: ", 0.0, std::numeric_limits<double>::max());
    util::Date *trans_date = get_date("transaction date: ");
    store::PurchaseTransaction *new_transaction = new store::PurchaseTransaction("", trans_date);
    inventory::PurchaseEntry *new_entry = new inventory::PurchaseEntry(item_code, new_transaction->get_db_code(), capitalized_amt, 1);
    new_transaction->add_entry(new_entry);
    double paid_cash = get_double("How much cash paid? ", 0, new_transaction->get_transaction_amount());
    new_transaction->set_paid_cash(paid_cash);
    new_transaction->set_paid_credit(new_transaction->get_transaction_amount() - paid_cash);
    new_transaction->insert_to_db();
    s_system->capitalize_asset(new_transaction);
}

void sell_inventory(store::StoreSystem *s_system)
{
    std::string item_code = get_string("inventory item code: ");
    int qty = get_int("qty: ", 1, std::numeric_limits<int>::max());
    double price = s_system->get_inventory(item_code)->get_selling_price();
    util::Date *date = get_date("enter date: ");
    store::SellingTransaction *new_transaction = new store::SellingTransaction(date);
    inventory::SellingEntry *new_entry = new inventory::SellingEntry(item_code, new_transaction->get_db_code(), price, qty);
    new_transaction->add_entry(new_entry);
    int more = get_int("more?", 0, 1);
    while (more)
    {
        item_code = get_string("inventory item code: ");
        qty = get_int("qty: ", 1, std::numeric_limits<int>::max());
        price = s_system->get_inventory(item_code)->get_selling_price();
        new_entry = new inventory::SellingEntry(item_code, new_transaction->get_db_code(), price, qty);
        new_transaction->add_entry(new_entry);
        more = get_int("more?", 0, 1);
    }
    double paid_cash = get_double("How much cash paid?", 0, new_transaction->get_transaction_amount());
    new_transaction->set_paid_cash(paid_cash);
    new_transaction->set_paid_credit(new_transaction->get_transaction_amount() - paid_cash);
    new_transaction->insert_to_db();
    s_system->sell_item(new_transaction);
}

void sell_assets(store::StoreSystem *s_system)
{
    std::string item_code = get_string("asset item code: ");
    double price = get_double("selling price", 0, std::numeric_limits<double>::max());
    util::Date *date = get_date("enter date: ");
    store::SellingTransaction *new_transaction = new store::SellingTransaction(date);
    
    inventory::SellingEntry *new_entry = new inventory::SellingEntry(item_code, new_transaction->get_db_code(), price, 1);
    new_transaction->add_entry(new_entry);
    double paid_cash = get_double("How much cash paid?", 0, new_transaction->get_transaction_amount());
    new_transaction->set_paid_cash(paid_cash);
    new_transaction->set_paid_credit(new_transaction->get_transaction_amount() - paid_cash);
    new_transaction->insert_to_db();
    s_system->dispose_asset(new_transaction);
}

void end_of_year(store::StoreSystem *s_system){
    s_system->end_year_adjustment();
}

void list_of_items(store::StoreSystem *s_system)
{
    std::cout << s_system->to_string_inv() << std::endl;
}