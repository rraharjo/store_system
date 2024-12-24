#include "driver/driver.hpp"
#include <cstring>

util::Date *get_date(std::string format)
{
    util::Date *to_ret = new util::Date(format);
    return to_ret;
}

void add_new_inventory(store::StoreSystem *s_system, std::vector<std::string> &tokenized_command)
{
    int args_size = tokenized_command.size();
    if (tokenized_command[args_size - 1] != ENDCMD)
    {
        throw std::runtime_error("missing end command token");
    }
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
    int args_size = tokenized_command.size();
    if (tokenized_command[args_size - 1] != ENDCMD)
    {
        throw std::runtime_error("missing end command token");
    }
    util::Date *trans_date = get_date(tokenized_command[1]);
    std::string seller = tokenized_command[2];
    store::PurchaseTransaction *new_transaction = new store::PurchaseTransaction(seller, trans_date);
    add_purchase_entry(new_transaction, tokenized_command[3], std::stod(tokenized_command[4]), std::stoi(tokenized_command[5]));
    int cur_idx = 6;
    while (cur_idx < args_size - 2)
    {
        add_purchase_entry(new_transaction, tokenized_command[cur_idx], std::stod(tokenized_command[cur_idx + 1]), std::stoi(tokenized_command[cur_idx + 2]));
        cur_idx += 3;
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
    int args_size = tokenized_command.size();
    if (tokenized_command[args_size - 1] != ENDCMD)
    {
        throw std::runtime_error("missing end command token");
    }
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
    int args_size = tokenized_command.size();
    if (tokenized_command[args_size - 1] != ENDCMD)
    {
        throw std::runtime_error("missing end command token");
    }
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
    int args_size = tokenized_command.size();
    if (tokenized_command[args_size - 1] != ENDCMD)
    {
        throw std::runtime_error("missing end command token");
    }
    util::Date *date = get_date(tokenized_command[1]);
    std::string item_code = tokenized_command[2];
    int qty = std::stoi(tokenized_command[3]);
    double price = s_system->get_inventory(item_code)->get_selling_price();

    store::SellingTransaction *new_transaction = new store::SellingTransaction(date);
    inventory::SellingEntry *new_entry = new inventory::SellingEntry(item_code, new_transaction->get_db_code(), price, qty);
    new_transaction->add_entry(new_entry);
    int cur_idx = 4;
    while (cur_idx < args_size - 2)
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
    int args_size = tokenized_command.size();
    if (tokenized_command[args_size - 1] != ENDCMD)
    {
        throw std::runtime_error("missing end command token");
    }
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

void end_of_year(store::StoreSystem *s_system, std::vector<std::string> &tokenized_command)
{
    int args_size = tokenized_command.size();
    if (tokenized_command[args_size - 1] != ENDCMD)
    {
        throw std::runtime_error("missing end command token");
    }
    s_system->end_year_adjustment();
}

/************************************************************ */

storedriver::Driver::Driver()
{
    this->s_system = store::StoreSystem::get_instance();
};

int storedriver::Driver::execute_command(std::string command)
{
    try
    {
        std::vector<std::string> tokenized_command = util::tokenize(command);
        int main_command = std::stoi(tokenized_command[0]);
        switch (main_command)
        {
        case 0:
            return 1;
        case ADD_INV:
            add_new_inventory(this->s_system, tokenized_command);
            return 1;
        case PURC_INV:
            purchase_inventory(this->s_system, tokenized_command);
            return 1;
        case PURC_ASS:
            purchase_asset(this->s_system, tokenized_command);
            return 1;
        case CAPT_ASS:
            capitalize_assets(this->s_system, tokenized_command);
            return 1;
        case SELL_INV:
            sell_inventory(this->s_system, tokenized_command);
            return 1;
        case SELL_ASS:
            sell_assets(this->s_system, tokenized_command);
            return 1;
        case EO_YEAR:
            end_of_year(this->s_system, tokenized_command);
            return 1;
        default:
            throw std::invalid_argument("Unknown command " + std::to_string(main_command));
        }
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return 0;
    }
}

int storedriver::Driver::execute_commands(std::string commands)
{
    std::vector<std::string> multiple_commands;
    std::string delimiter = ENDCMD;
    std::string temp;
    int commands_length = commands.length(), delimiter_length = delimiter.length();
    int j = 0;
    for (int i = 0; i < commands_length; i++)
    {
        if (commands[i] == delimiter[j])
        {
            j++;
        }
        else
        {
            j = 0;
        }
        temp.push_back(commands[i]);
        if (j == delimiter_length)
        {
            multiple_commands.push_back(temp);
            temp.clear();
        }
    }
    int to_ret = 0;
    for (std::string command : multiple_commands)
    {
        to_ret += this->execute_command(command);
    }
    return to_ret;
}

storedriver::StdIODriver::StdIODriver() : storedriver::Driver()
{
}

void storedriver::StdIODriver::start()
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
        ongoing = this->execute_commands(buff);
    }
}

int storedriver::PipeIODriver::execute_commands(std::string commands)
{
    std::vector<std::string> multiple_commands;
    std::string delimiter = ENDCMD;
    std::string temp;
    int commands_length = commands.length(), delimiter_length = delimiter.length();
    int j = 0;
    for (int i = 0; i < commands_length; i++)
    {
        if (commands[i] == delimiter[j])
        {
            j++;
        }
        else
        {
            j = 0;
        }
        temp.push_back(commands[i]);
        if (j == delimiter_length)
        {
            multiple_commands.push_back(temp);
            temp.clear();
        }
    }
    int to_ret = 0;
    for (std::string command : multiple_commands)
    {
        if (this->execute_command(command))
        {
            to_ret++;
            this->write_output("success");
        }
        else
        {
            this->write_output("failed");
        }
    }
    return to_ret;
}

storedriver::PipeIODriver::PipeIODriver() : storedriver::Driver()
{
    if (_pipe(this->input_pipe, 2 * STREAM_SIZE, O_TEXT) == -1)
    {
        throw std::runtime_error("error on input _pipe()");
    }
    if (_pipe(this->output_pipe, 2 * STREAM_SIZE, O_TEXT) == -1)
    {
        throw std::runtime_error("error on output _pipe()");
    }
}

std::string storedriver::PipeIODriver::read_input()
{
    char read_buff[STREAM_SIZE];
    int bytes_read = 0;
    if (bytes_read = _read(this->input_pipe[0], read_buff, STREAM_SIZE) == -1)
    {
        throw std::runtime_error("error on read_input _read()");
    }
    std::string to_ret(read_buff);
    return to_ret;
}

void storedriver::PipeIODriver::write_input(std::string input)
{
    int input_length = input.length();
    if (input_length > STREAM_SIZE - 1)
    {
        throw std::invalid_argument("error on write_input - input is too long");
    }
    char write_buff[STREAM_SIZE];
    strcpy(write_buff, input.c_str());
    if (_write(this->input_pipe[1], write_buff, input_length + 1) == -1)
    {
        throw std::runtime_error("error on write_input _write()");
    }
}

std::string storedriver::PipeIODriver::read_output()
{
    char read_buff[STREAM_SIZE];
    int bytes_read = 0;
    std::string to_ret;
    if (bytes_read = _read(this->output_pipe[0], read_buff, STREAM_SIZE) == -1)
    {
        throw std::runtime_error("error on read_output _read()");
    }
    to_ret = std::string(read_buff);
    return to_ret;
}

void storedriver::PipeIODriver::write_output(std::string input)
{
    int input_length = input.length();
    if (input_length > STREAM_SIZE - 1)
    {
        throw std::invalid_argument("error on write_output - input is too long");
    }
    char write_buff[STREAM_SIZE];
    strcpy(write_buff, input.c_str());
    if (_write(this->output_pipe[1], write_buff, input_length + 1) == -1)
    {
        throw std::runtime_error("error on write_output _write()");
    }
}

void storedriver::PipeIODriver::start()
{
    int read_bytes = 0;
    std::string input = "";
    while (true)
    {
        input.clear();
        input = this->read_input();
        this->execute_commands(input);
    }
}
