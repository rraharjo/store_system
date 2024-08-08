#include <deque>
#include "inventory/assets/sellable.hpp"

using namespace inventory;

Sellable::Sellable(std::string name, std::string itemCode, double sellingPrice) : Item::Item(name, itemCode)
{
    this->sellingPrice = sellingPrice;
    this->qty = 0;
    this->history = new PurchaseHistory();
    // needs refactoring
    std::vector<std::vector<std::string>> result = util::DB::get_instance()->execute_query(
        "insert into sellable(item_code, item_name, selling_price) values('" +
        this->itemCode + "', '" + this->name + "', " + std::to_string(this->sellingPrice) + ") returning *;");
    // insert to database
    this->databaseCode = std::stoi(result[0][0]); // take from database
}

int Sellable::getDBCode(){
    return this->databaseCode;
}

double Sellable::getSellingPrice()
{
    return this->sellingPrice;
}

void Sellable::setSellingPrice(double newPrice)
{
    this->sellingPrice = newPrice;
}

double Sellable::removeItem(int qty)
{
    if (this->qty < qty){
        throw std::invalid_argument("Purchasing quantity exceeds available quantity");
        return -1;
    }
    this->qty -= qty;
    return this->history->sellItemFirstIn(qty);
}

void Sellable::addPurchase(TransactionEntry *entry)
{
    this->qty += entry->qty;
    this->history->addEntry(entry);
    std::vector<std::vector<std::string>> result = util::DB::get_instance()->execute_query(
        "insert into purchase_entry(sellable_db_code, purchase_db_code, purchase_price, qty, available_qty) values(" + 
        std::to_string(this->databaseCode) + ", " + std::to_string(entry->transactionDBCode) + ", " +
        std::to_string(entry->price) + ", " + std::to_string(entry->qty) + ", " + std::to_string(entry->qty) + 
        ") returning *;"
    );
}

std::string Sellable::to_string(){
    std::string toRet = "";
    toRet += "DB code: " + std::to_string(this->databaseCode) + "\n";
    toRet += "item code: " + this->itemCode + "\n";
    toRet += "name: " + this->name + "\n";
    toRet += "qty: " + std::to_string(this->qty) + "\n";
    return toRet;
}