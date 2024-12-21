#include "driver/driver.hpp"
#include <cstring>

util::Date *get_date(std::string format)
{
    util::Date *to_ret = new util::Date(format);
    return to_ret;
}

void add_new_inventory(store::StoreSystem *s_system, std::vector<std::string> &tokenized_command)
{
    // format: 1 "Product name" "item code" selling_price
    std::string name = tokenized_command[1];
    std::string item_code = tokenized_command[2];
    double price = std::stod(tokenized_command[3]);
    inventory::Inventory *new_inventory = new inventory::Inventory(item_code, name, price);
    new_inventory->insert_to_db();
    s_system->add_item(new_inventory);
    return;
}

void add_purchase_entry(store::PurchaseTransaction *p, std::string item_db, double price_per_item, int item_qty)
{
    std::string item_code = item_db;
    double price = price_per_item;
    int qty = item_qty;
    inventory::PurchaseEntry *new_entry = new inventory::PurchaseEntry(item_code, p->get_db_code(), price, qty);
    p->add_entry(new_entry);
    return;
}

void purchase_inventory(store::StoreSystem *s_system, std::vector<std::string> &tokenized_command)
{
    // format: 2 date "Seller_name" item_db price_each qty [...item_db price_each qty] paid_cash
    int total_args = tokenized_command.size();
    util::Date *trans_date = get_date(tokenized_command[1]);
    std::string seller = tokenized_command[2];
    store::PurchaseTransaction *new_transaction = new store::PurchaseTransaction(seller, trans_date);
    add_purchase_entry(new_transaction, tokenized_command[3], std::stod(tokenized_command[4]), std::stoi(tokenized_command[5]));
    int cur_idx = 6;
    while (cur_idx < total_args - 1)
    {
        add_purchase_entry(new_transaction, tokenized_command[cur_idx++], std::stod(tokenized_command[cur_idx++]), std::stoi(tokenized_command[cur_idx++]));
    }
    double paid_cash = std::stod(tokenized_command[cur_idx]);
    new_transaction->set_paid_cash(paid_cash);
    new_transaction->set_paid_credit(new_transaction->get_transaction_amount() - paid_cash);
    new_transaction->insert_to_db();
    s_system->buy_item(new_transaction);
    return;
}

void purchase_asset(store::StoreSystem *s_system, std::vector<std::string> &tokenized_command)
{
    // format: 3 date "eq_name" "item_code" cost residual yr_useful paid_cash
    util::Date *date_purchased = get_date(tokenized_command[1]);
    std::string name = tokenized_command[2];
    std::string item_code = tokenized_command[3];
    double purchase_cost = std::stod(tokenized_command[4]);
    double residual_value = std::stod(tokenized_command[5]);
    int useful_life = std::stod(tokenized_command[6]);

    inventory::Equipment *new_eqp = new inventory::Equipment(name, item_code, residual_value, useful_life, date_purchased);
    new_eqp->insert_to_db();
    store::PurchaseTransaction *new_transaction = new store::PurchaseTransaction("", date_purchased);
    inventory::PurchaseEntry *new_entry = new inventory::PurchaseEntry(new_eqp->get_db_code(), "", purchase_cost, 1);
    new_transaction->add_entry(new_entry);
    double paid_cash = std::stod(tokenized_command[7]);
    new_transaction->set_paid_cash(paid_cash);
    new_transaction->set_paid_credit(new_transaction->get_transaction_amount() - paid_cash);
    new_transaction->insert_to_db();
    s_system->add_property(new_eqp);
    s_system->capitalize_asset(new_transaction);
}

void capitalize_assets(store::StoreSystem *s_system, std::vector<std::string> &tokenized_command)
{
    // format: 4 date "db_code" capt_amt  paid_cash
    util::Date *trans_date = get_date(tokenized_command[1]);
    std::string item_db_code = tokenized_command[2];
    double capitalized_amt = std::stod(tokenized_command[3]);

    store::PurchaseTransaction *new_transaction = new store::PurchaseTransaction("", trans_date);
    inventory::PurchaseEntry *new_entry = new inventory::PurchaseEntry(item_db_code, new_transaction->get_db_code(), capitalized_amt, 1);
    new_transaction->add_entry(new_entry);
    double paid_cash = std::stod(tokenized_command[4]);
    new_transaction->set_paid_cash(paid_cash);
    new_transaction->set_paid_credit(new_transaction->get_transaction_amount() - paid_cash);
    new_transaction->insert_to_db();
    s_system->capitalize_asset(new_transaction);
}

void sell_inventory(store::StoreSystem *s_system, std::vector<std::string> &tokenized_command)
{
    // format: 5 date db_code qty
    int args_size = tokenized_command.size();
    util::Date *date = get_date(tokenized_command[1]);
    std::string item_code = tokenized_command[2];
    int qty = std::stoi(tokenized_command[3]);
    double price = s_system->get_inventory(item_code)->get_selling_price();

    store::SellingTransaction *new_transaction = new store::SellingTransaction(date);
    inventory::SellingEntry *new_entry = new inventory::SellingEntry(item_code, new_transaction->get_db_code(), price, qty);
    new_transaction->add_entry(new_entry);
    int cur_idx = 4;
    while (cur_idx < args_size - 1)
    {
        item_code = tokenized_command[cur_idx++];
        qty = std::stoi(tokenized_command[cur_idx++]);
        price = s_system->get_inventory(item_code)->get_selling_price();
        new_entry = new inventory::SellingEntry(item_code, new_transaction->get_db_code(), price, qty);
        new_transaction->add_entry(new_entry);
    }
    double paid_cash = std::stod(tokenized_command[cur_idx]);
    new_transaction->set_paid_cash(paid_cash);
    new_transaction->set_paid_credit(new_transaction->get_transaction_amount() - paid_cash);
    new_transaction->insert_to_db();
    s_system->sell_item(new_transaction);
}

void sell_assets(store::StoreSystem *s_system, std::vector<std::string> &tokenized_command)
{
    // format: 6 date db_code price paid_cash
    util::Date *date = get_date(tokenized_command[1]);
    std::string item_code = tokenized_command[2];
    double price = std::stod(tokenized_command[3]);

    store::SellingTransaction *new_transaction = new store::SellingTransaction(date);
    inventory::SellingEntry *new_entry = new inventory::SellingEntry(item_code, new_transaction->get_db_code(), price, 1);
    new_transaction->add_entry(new_entry);
    double paid_cash = std::stod(tokenized_command[4]);
    new_transaction->set_paid_cash(paid_cash);
    new_transaction->set_paid_credit(new_transaction->get_transaction_amount() - paid_cash);
    new_transaction->insert_to_db();
    s_system->dispose_asset(new_transaction);
}

void end_of_year(store::StoreSystem *s_system)
{
    s_system->end_year_adjustment();
}

/************************************************************ */

storedriver::Driver::Driver()
{
    this->s_system = store::StoreSystem::get_instance();
};

bool storedriver::Driver::execute_command(std::string command)
{
    std::vector<std::string> tokenized_command = util::tokenize(command);
    int main_command = std::stoi(tokenized_command[0]);
    // TODO test the new driver
    switch (main_command)
    {
    case 0:
        return false;
    case ADD_INV:
        add_new_inventory(this->s_system, tokenized_command);
        return true;
    case PURC_INV:
        purchase_inventory(this->s_system, tokenized_command);
        return true;
    case PURC_ASS:
        purchase_asset(this->s_system, tokenized_command);
        return true;
    case CAPT_ASS:
        capitalize_assets(this->s_system, tokenized_command);
        return true;
    case SELL_INV:
        sell_inventory(this->s_system, tokenized_command);
        return true;
    case SELL_ASS:
        sell_assets(this->s_system, tokenized_command);
        return true;
    case EO_YEAR:
        end_of_year(this->s_system);
        return true;
    default:
        throw std::invalid_argument("unknown command " + std::to_string(main_command));
    }
}

storedriver::StdDriver::StdDriver() : storedriver::Driver()
{
}

void storedriver::StdDriver::start()
{
    char buff[BUFSIZ];
    int buff_size;
    std::string buff_str;
    bool ongoing = true;
    while (ongoing)
    {
        std::cout << '>';
        std::cin.getline(buff, BUFSIZ);
        buff_size = strlen(buff);
        buff_str = std::string(buff);
        if (!buff_size)
        {
            break;
        }
        try
        {
            ongoing = this->execute_command(buff);
        }
        catch (std::runtime_error &e)
        {
            std::cout << e.what() << std::endl;
        }
        catch (std::invalid_argument &e)
        {
            std::cout << e.what() << std::endl;
        }
    }
}

storedriver::CustomDriver::CustomDriver() : storedriver::Driver()
{
}

// TODO fix custom stream use _read _pipe for blocking behavior
void storedriver::CustomDriver::start()
{
}
