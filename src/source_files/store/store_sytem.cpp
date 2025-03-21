#include "store/store_system.hpp"
using namespace store;

void check_transaction(Transaction *transaction)
{
    double total_amt = 0;
    for (std::shared_ptr<inventory::Entry> entry : transaction->get_all_entries())
    {
        total_amt += entry->get_price() * entry->get_qty();
    }
    if (total_amt != transaction->get_paid_cash() + transaction->get_paid_credit())
    {
        throw std::invalid_argument("Amount paid does not equal to the purchase price; paid credit: " + std::to_string(transaction->get_paid_credit()) + " paid cash: " + std::to_string(transaction->get_paid_cash()) + " purchase amount: " + std::to_string(total_amt));
    }
}

std::unique_ptr<StoreSystem> StoreSystem::instance = NULL;

StoreSystem *StoreSystem::get_instance()
{
    if (StoreSystem::instance.get() == NULL)
    {
        StoreSystem::instance.reset(new StoreSystem());
    }
    return StoreSystem::instance.get();
}

StoreSystem::StoreSystem()
{
    this->a_system = accounting::AccountingSystem::get_instance();
    this->i_system = inventory::InventorySystem::get_instance();
    this->purchase_transactions.reset(new util::baseclass::PurchaseTransactionCollection());
    this->selling_transactions.reset(new util::baseclass::SellingTransactionCollection());
}

void StoreSystem::sell_item(SellingTransaction *selling_transaction)
{
    check_transaction(selling_transaction);
    this->selling_transactions->insert_new_item(selling_transaction);
    double cogs = 0;
    double sell_amount = 0;
    for (std::shared_ptr<inventory::Entry> entry : selling_transaction->get_all_entries())
    {
        sell_amount += entry->get_price() * entry->get_qty();
        cogs += this->i_system->sell_sellables(entry);
    }
    util::Date *transaction_date = new util::Date();
    std::string inc_rev_desc = "Selling inventory";
    std::string inc_cogs_desc = "Increase cost of goods sold";
    accounting::Transaction *acct_transaction =
        util::factory::GoodsSellingFactory(transaction_date, inc_rev_desc, selling_transaction->get_db_code(), sell_amount,
                                           selling_transaction->get_paid_cash(), selling_transaction->get_paid_credit())
            .create_transaction();
    accounting::Transaction *acct_transaction_2 =
        util::factory::GoodsSoldCOGSFactory(transaction_date, inc_cogs_desc, selling_transaction->get_db_code(), cogs)
            .create_transaction();
    this->a_system->add_transaction(acct_transaction);
    this->a_system->add_transaction(acct_transaction_2);
    delete acct_transaction;
    delete acct_transaction_2;
}

void StoreSystem::buy_item(PurchaseTransaction *purchase_transaction)
{
    check_transaction(purchase_transaction);
    this->purchase_transactions->insert_new_item(purchase_transaction);
    double purchase_amount = 0;
    // entry does not have inventory code
    for (std::shared_ptr<inventory::Entry> entry : purchase_transaction->get_all_entries())
    {
        purchase_amount += entry->get_price() * entry->get_qty();
        this->i_system->purchase_sellables(entry);
    }
    util::Date *transaction_date = new util::Date();
    std::string description = "Purchase inventory";
    accounting::Transaction *acct_transaction =
        util::factory::GoodsPurchaseFactory(transaction_date, description, purchase_transaction->get_db_code(),
                                            purchase_amount, purchase_transaction->get_paid_cash(), purchase_transaction->get_paid_credit())
            .create_transaction();
    this->a_system->add_transaction(acct_transaction);
    delete acct_transaction;
}

void StoreSystem::capitalize_asset(PurchaseTransaction *purchase_transaction)
{
    check_transaction(purchase_transaction);
    double amount = 0.0;
    this->purchase_transactions->insert_new_item(purchase_transaction);
    for (std::shared_ptr<inventory::Entry> entry : purchase_transaction->get_all_entries())
    {
        amount += entry->get_price();
        this->i_system->purchase_properties(entry);
        //amount is wrong
        //entry is deleted on purchase_properties
    }
    util::Date *transaction_date = new util::Date();
    std::string description = "Purchase asset";
    accounting::Transaction *acct_transaction =
        util::factory::BuyEquipmentFactory(transaction_date, description, purchase_transaction->get_db_code(),
                                           amount, purchase_transaction->get_paid_cash(), purchase_transaction->get_paid_credit())
            .create_transaction();
    this->a_system->add_transaction(acct_transaction);
    delete acct_transaction;
}
void StoreSystem::dispose_asset(SellingTransaction *selling_transaction)
{ // one transaction one property
    check_transaction(selling_transaction);
    this->selling_transactions->insert_new_item(selling_transaction);
    inventory::Equipment *to_dispose = NULL;
    double sell_amount = 0.0;
    double prop_valuation = 0.0;
    for (std::shared_ptr<inventory::Entry> entry : selling_transaction->get_all_entries())
    {
        sell_amount += entry->get_price();
        entry->set_transaction_date(selling_transaction->get_date());
        prop_valuation += this->i_system->sell_properties(entry);
        to_dispose = (inventory::Equipment *)this->i_system->get_property(entry->get_properties_db_code()); // possible mem_leak
    }
    util::Date *transaction_date = new util::Date();
    std::string description = "Asset disposal";
    accounting::Transaction *acct_transaction =
        util::factory::SellEquipmentFactory(transaction_date, description, selling_transaction->get_db_code(),
                                            to_dispose->get_current_accumulated_depreciation(), prop_valuation,
                                            selling_transaction->get_paid_cash(), selling_transaction->get_paid_credit())
            .create_transaction();
    this->a_system->add_transaction(acct_transaction);
    delete to_dispose;
    delete acct_transaction;
}

void StoreSystem::add_item(inventory::Inventory *new_sellable)
{
    this->i_system->add_new_item(new_sellable);
}

void StoreSystem::add_property(inventory::Equipment *new_prop)
{
    this->i_system->add_new_property(new_prop);
}

void StoreSystem::end_year_adjustment()
{
    this->i_system->apply_all_depreciation();
    this->a_system->end_year_adjustment();
}

inventory::Inventory *StoreSystem::get_inventory(std::string db_code)
{
    return this->i_system->get_inventory(db_code);
}

std::vector<inventory::Inventory *> StoreSystem::get_inventory()
{
    return this->i_system->get_inventory();
}

std::vector<inventory::Asset *> StoreSystem::get_assets()
{
    return this->i_system->get_assets();
}

std::string StoreSystem::to_string_inv()
{
    return this->i_system->to_string();
}

std::string StoreSystem::to_string()
{
    std::string to_ret = "";
    to_ret += this->i_system->to_string();
    to_ret += this->a_system->to_string();
    return to_ret;
}