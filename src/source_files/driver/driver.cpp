#include "driver/driver.hpp"

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

void add_purchase_entry(store::PurchaseTransaction *p, std::string item_db, double price_per_item, int qty)
{
    std::string item_code = item_db;
    double price = price_per_item;
    int qty = qty;
    inventory::PurchaseEntry *new_entry = new inventory::PurchaseEntry(item_code, p->get_db_code(), price, qty);
    p->add_entry(new_entry);
    return;
}

void purchase_inventory(store::StoreSystem *s_system, std::vector<std::string> &tokenized_command)
{
    // format: 2 "Seller_name" transaction_date item_db price_each qty [...item_db price_each qty] paid_cash paid_credit
    int total_args = tokenized_command.size();
    std::string seller = tokenized_command[1];
    util::Date *trans_date = get_date(tokenized_command[2]);
    store::PurchaseTransaction *new_transaction = new store::PurchaseTransaction(seller, trans_date);
    add_purchase_entry(new_transaction, tokenized_command[3], std::stod(tokenized_command[4]), std::stoi(tokenized_command[5]));
    int cur_idx = 6;
    while (cur_idx < total_args - 2)
    {
        add_purchase_entry(new_transaction, tokenized_command[cur_idx++], std::stod(tokenized_command[cur_idx++]), std::stoi(tokenized_command[cur_idx++]));
    }
    double paid_cash = std::stod(tokenized_command[cur_idx++]);
    new_transaction->set_paid_cash(paid_cash);
    new_transaction->set_paid_credit(new_transaction->get_transaction_amount() - paid_cash);
    new_transaction->insert_to_db();
    s_system->buy_item(new_transaction);
    return;
}

storedriver::Driver::Driver()
{
    this->s_system = store::StoreSystem::get_instance();
};

void storedriver::Driver::execute_command(std::string command)
{
    std::vector<std::string> tokenized_command = util::tokenize(command);
    int main_command = std::stoi(tokenized_command[0]);
    switch (main_command)
    {
    case 0:
        break;
    case ADD_INV:
        break;
    case PURC_INV:
        break;
    case PURC_ASS:
        break;
    case CAPT_ASS:
        break;
    case SELL_INV:
        break;
    case SELL_ASS:
        break;
    case EO_YEAR:
        break;
    case LIST_ITEM:
        break;
    default:
        throw std::runtime_error("unknown command " + std::to_string(main_command));
    }
}

void storedriver::StdDriver::start()
{
    char buff[STREAM_SIZE];
    int buff_size;
    int in_size;
    while (true)
    {
        std::cin.get(buff, STREAM_SIZE);
        buff_size = strlen(buff);
        if (!buff_size)
        {
            break;
        }
        std::string str_buff = std::string(buff);
        try
        {
            this->execute_command(str_buff);
        }
        catch (std::exception e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}

// TODO fix custom stream
void storedriver::CustomDriver::start()
{
    char buff[STREAM_SIZE];
    while (this->input_stream.read(buff, STREAM_SIZE))
    {
        std::string str_buff = std::string(buff);
        try
        {
            this->execute_command(str_buff);
            this->output_stream << str_buff << " success";
        }
        catch (std::exception e)
        {
            this->output_stream << str_buff << " failed: " << e.what();
        }
    }
}
