#include "driver/executor.hpp"

nlohmann::json build_json_from_untokenized_command(const std::string &raw_command)
{
    nlohmann::json to_ret;
    std::vector<std::string> tokenized_command = util::tokenize(raw_command);
    int main_command = std::stoi(tokenized_command[0]);
    int args_size = tokenized_command.size();

    if (tokenized_command[args_size - 1] != ENDCMD)
    {
        throw std::runtime_error("missing end command token");
    }

    to_ret["main_command"] = main_command;
    switch (main_command)
    {
    case ADD_INV:
        // format 1 product_name item_code price
        to_ret["product_name"] = tokenized_command[1];
        to_ret["item_code"] = tokenized_command[2];
        to_ret["price"] = std::stod(tokenized_command[3]);
        break;
    case PURC_INV:
    {
        // format: 2 date seller_name [item_db price_each qty]+ paid_cash ENDCMD
        to_ret["date"] = tokenized_command[1];
        to_ret["seller"] = tokenized_command[2];
        int cur_idx = 3;
        std::vector<nlohmann::json> items;
        while (cur_idx < tokenized_command.size() - 2)
        {
            nlohmann::json item;
            item["dbcode"] = tokenized_command[cur_idx++];
            item["price"] = std::stod(tokenized_command[cur_idx++]);
            item["qty"] = std::stoi(tokenized_command[cur_idx++]);
            items.push_back(item);
        }
        to_ret["items"] = items;
        to_ret["paid_cash"] = std::stod(tokenized_command[cur_idx]);
        break;
    }
    case PURC_ASS:
        // format: 3 date eq_name item_code cost residual yr_useful paid_cash ENDCMD
        to_ret["date"] = tokenized_command[1];
        to_ret["name"] = tokenized_command[2];
        to_ret["item_code"] = tokenized_command[3];
        to_ret["cost"] = std::stod(tokenized_command[4]);
        to_ret["residual_value"] = std::stod(tokenized_command[5]);
        to_ret["useful_life"] = std::stoi(tokenized_command[6]);
        to_ret["paid_cash"] = std::stod(tokenized_command[7]);
        break;
    case CAPT_ASS:
        //-format: 4 date db_code capt_amt paid_cash ENDCMD
        to_ret["date"] = tokenized_command[1];
        to_ret["dbcode"] = tokenized_command[2];
        to_ret["cost"] = std::stod(tokenized_command[3]);
        to_ret["paid_cash"] = std::stod(tokenized_command[4]);
        break;
    case SELL_INV:
    { //-format: 5 date [db_code qty]+ paid_cash ENDCMD
        to_ret["date"] = tokenized_command[1];
        std::vector<nlohmann::json> items;
        int cur_idx = 2;
        while (cur_idx < args_size - 2)
        {
            nlohmann::json item;
            item["dbcode"] = tokenized_command[cur_idx++];
            item["qty"] = std::stoi(tokenized_command[cur_idx++]);
            items.push_back(item);
        }
        to_ret["items"] = items;
        to_ret["paid_cash"] = std::stod(tokenized_command[cur_idx]);
        break;
    }
    case SELL_ASS:
        //-format: 6 date db_code price paid_cash ENDCMD
        to_ret["date"] = tokenized_command[1];
        to_ret["dbcode"] = tokenized_command[2];
        to_ret["price"] = std::stod(tokenized_command[3]);
        to_ret["paid_cash"] = std::stod(tokenized_command[4]);
        break;
    case EO_YEAR:
        //-format: 7 ENDCMD
        break;
    case INV_INFO:
        //-format: 8 ENDCMD
        break;
    default:
        throw std::invalid_argument("Unknown command " + raw_command);
        break;
    }
    return to_ret;
}

nlohmann::json storedriver::Executor::execute(store::StoreSystem *s_system, std::string raw_command, bool json)
{
    int main_command;
    nlohmann::json exec, to_ret;
    storedriver::Executor *e = NULL;
    if (json)
    {
        exec = nlohmann::json::parse(raw_command);
    }
    else
    {
        exec = build_json_from_untokenized_command(raw_command);
    }
    main_command = exec.at("main_command");
    switch (main_command)
    {
    case ADD_INV:
        e = new AddInventoryExecutor(exec);
        break;
    case PURC_INV:
        e = new PurchaseInventoryExecutor(exec);
        break;
    case PURC_ASS:
        e = new PurchaseAssetsExecutor(exec);
        break;
    case CAPT_ASS:
        e = new CapitalizeAssetExecutor(exec);
        break;
    case SELL_INV:
        e = new SellInventoryExecutor(exec);
        break;
    case SELL_ASS:
        e = new SellAssetExecutor(exec);
        break;
    case EO_YEAR:
        e = new EndOfYearExecutor(exec);
        break;
    case INV_INFO:
        e = new InventoriesInfoExecutor(exec);
        break;
    default:
        throw std::invalid_argument("Unknown command: " + raw_command);
        break;
    }
    to_ret = e->execute(s_system);
    delete e;
    return to_ret;
}

storedriver::Executor::Executor(nlohmann::json json_command)
{
    this->request = json_command;
}

storedriver::AddInventoryExecutor::AddInventoryExecutor(nlohmann::json json_command) : Executor(json_command) {}

nlohmann::json storedriver::AddInventoryExecutor::execute(store::StoreSystem *s_system)
{
    std::string name = this->request.at("product_name");
    std::string item_code = this->request.at("item_code");
    double price = (double)this->request.at("price");
    inventory::Inventory *new_inventory = new inventory::Inventory(item_code, name, price);
    new_inventory->insert_to_db();
    s_system->add_item(new_inventory);
    return nlohmann::json(R"({})"_json);
}

storedriver::PurchaseInventoryExecutor::PurchaseInventoryExecutor(nlohmann::json json_command) : Executor(json_command) {}

void storedriver::PurchaseInventoryExecutor::add_purchase_entry(store::PurchaseTransaction *p, std::string item_db, double price_per_item, int item_qty)
{
    std::string item_code = item_db;
    double price = price_per_item;
    int qty = item_qty;
    inventory::PurchaseEntry *new_entry = new inventory::PurchaseEntry(item_code, p->get_db_code(), price, qty);
    p->add_entry(new_entry);
    return;
}

nlohmann::json storedriver::PurchaseInventoryExecutor::execute(store::StoreSystem *s_system)
{
    util::Date *trans_date = new util::Date(this->request.at("date"));
    std::string seller = this->request.at("seller");
    store::PurchaseTransaction *new_transaction = new store::PurchaseTransaction(seller, trans_date);
    std::vector<nlohmann::json> items = this->request.at("items");
    for (const nlohmann::json &item : items)
    {
        add_purchase_entry(new_transaction, item.at("dbcode"), (double)item.at("price"), item.at("qty"));
    }
    double paid_cash = (double)this->request.at("paid_cash");
    new_transaction->set_paid_cash(paid_cash);
    new_transaction->set_paid_credit(new_transaction->get_transaction_amount() - paid_cash);
    new_transaction->insert_to_db();
    s_system->buy_item(new_transaction);
    return nlohmann::json(R"({})"_json);
}

storedriver::PurchaseAssetsExecutor::PurchaseAssetsExecutor(nlohmann::json json_command) : Executor(json_command) {}
nlohmann::json storedriver::PurchaseAssetsExecutor::execute(store::StoreSystem *s_system)
{
    util::Date *date_purchased = new util::Date(this->request.at("date"));
    std::string name = this->request.at("name");
    std::string item_code = this->request.at("item_code");
    double purchase_cost = (double)this->request.at("cost");
    double residual_value = (double)this->request.at("residual_value");
    int useful_life = this->request.at("useful_life");

    inventory::Equipment *new_eqp = new inventory::Equipment(name, item_code, residual_value, useful_life, date_purchased);
    new_eqp->insert_to_db();
    store::PurchaseTransaction *new_transaction = new store::PurchaseTransaction("", date_purchased);
    inventory::PurchaseEntry *new_entry = new inventory::PurchaseEntry(new_eqp->get_db_code(), "", purchase_cost, 1);
    new_transaction->add_entry(new_entry);
    double paid_cash = (double)this->request.at("paid_cash");
    new_transaction->set_paid_cash(paid_cash);
    new_transaction->set_paid_credit(new_transaction->get_transaction_amount() - paid_cash);
    new_transaction->insert_to_db();
    s_system->add_property(new_eqp);
    s_system->capitalize_asset(new_transaction);
    return nlohmann::json(R"({})"_json);
}

storedriver::CapitalizeAssetExecutor::CapitalizeAssetExecutor(nlohmann::json json_command) : Executor(json_command) {}
nlohmann::json storedriver::CapitalizeAssetExecutor::execute(store::StoreSystem *s_system)
{
    util::Date *trans_date = new util::Date(this->request.at("date"));
    std::string item_db_code = this->request.at("dbcode");
    double capitalized_amt = (double)this->request.at("cost");

    store::PurchaseTransaction *new_transaction = new store::PurchaseTransaction("", trans_date);
    inventory::PurchaseEntry *new_entry = new inventory::PurchaseEntry(item_db_code, new_transaction->get_db_code(), capitalized_amt, 1);
    new_transaction->add_entry(new_entry);
    double paid_cash = (double)this->request.at("paid_cash");
    new_transaction->set_paid_cash(paid_cash);
    new_transaction->set_paid_credit(new_transaction->get_transaction_amount() - paid_cash);
    new_transaction->insert_to_db();
    s_system->capitalize_asset(new_transaction);
    return nlohmann::json(R"({})"_json);
}

storedriver::SellInventoryExecutor::SellInventoryExecutor(nlohmann::json json_command) : Executor(json_command) {}
nlohmann::json storedriver::SellInventoryExecutor::execute(store::StoreSystem *s_system)
{
    util::Date *date = new util::Date(request.at("date"));
    std::vector<nlohmann::json> items = request.at("items");
    store::SellingTransaction *new_transaction = new store::SellingTransaction(date);
    for (const nlohmann::json &item : items)
    {
        std::string item_code = item.at("dbcode");
        int qty = item.at("qty");
        double price = s_system->get_inventory(item_code)->get_selling_price();
        inventory::SellingEntry *new_entry = new inventory::SellingEntry(item_code, new_transaction->get_db_code(), price, qty);
        new_transaction->add_entry(new_entry);
    }
    double paid_cash = (double)this->request.at("paid_cash");
    new_transaction->set_paid_cash(paid_cash);
    new_transaction->set_paid_credit(new_transaction->get_transaction_amount() - paid_cash);
    new_transaction->insert_to_db();
    s_system->sell_item(new_transaction);
    return nlohmann::json(R"({})"_json);
}

storedriver::SellAssetExecutor::SellAssetExecutor(nlohmann::json json_command) : Executor(json_command) {}
nlohmann::json storedriver::SellAssetExecutor::execute(store::StoreSystem *s_system)
{
    util::Date *date = new util::Date(this->request.at("date"));
    std::string item_code = this->request.at("dbcode");
    double price = (double)this->request.at("price");

    store::SellingTransaction *new_transaction = new store::SellingTransaction(date);
    inventory::SellingEntry *new_entry = new inventory::SellingEntry(item_code, new_transaction->get_db_code(), price, 1);
    new_transaction->add_entry(new_entry);
    double paid_cash = (double)this->request.at("paid_cash");
    new_transaction->set_paid_cash(paid_cash);
    new_transaction->set_paid_credit(new_transaction->get_transaction_amount() - paid_cash);
    new_transaction->insert_to_db();
    s_system->dispose_asset(new_transaction);
    return nlohmann::json(R"({})"_json);
}

storedriver::EndOfYearExecutor::EndOfYearExecutor(nlohmann::json json_command) : Executor(json_command) {}
nlohmann::json storedriver::EndOfYearExecutor::execute(store::StoreSystem *s_system)
{
    s_system->end_year_adjustment();
    return nlohmann::json(R"({})"_json);
}

storedriver::InventoriesInfoExecutor::InventoriesInfoExecutor(nlohmann::json json_command) : Executor(json_command) {}
nlohmann::json storedriver::InventoriesInfoExecutor::execute(store::StoreSystem *s_system)
{
    nlohmann::json to_ret;
    std::vector<nlohmann::json> data;
    std::vector<inventory::Inventory *> inventories = s_system->get_inventory();
    for (inventory::Inventory *item : inventories)
    {
        nlohmann::json item_json;
        item_json["dbcode"] = item->get_db_code();
        item_json["item_code"] = item->get_item_code();
        item_json["name"] = item->get_name();
        item_json["price"] = item->get_selling_price();
        data.push_back(item_json);
    }
    to_ret["data"] = data;
    return to_ret;
}