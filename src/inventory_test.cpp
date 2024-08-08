#include "inventory/inventory_system.hpp"
using namespace inventory;
int main(){
    inventory::InventorySystem *system = new inventory::InventorySystem();
    Sellable *sellable1 = new Sellable("Drill battery", "DB123", 100.00);
    Sellable *sellable2 = new Sellable("JackHammer", "JH123", 120.00);
    int dbCode1 = sellable1->getDBCode();
    int dbCode2 = sellable2->getDBCode();
    system->addNewItem(sellable1);
    system->addNewItem(sellable2);
    TransactionEntry *entry1 = new TransactionEntry();
    entry1->itemDBCode = dbCode1;
    entry1->purchaseDate = new util::Date();
    entry1->price = 90.00;
    entry1->qty = 5;
    entry1->transactionDBCode = 1;
    TransactionEntry *entry2 = new TransactionEntry();
    entry2->itemDBCode = dbCode1;
    entry2->purchaseDate = new util::Date();
    entry2->price = 80.00;
    entry2->qty = 5;
    entry2->transactionDBCode = 1;
    std::cout << system->to_string() << std::endl;
    system->purchaseItem(entry1);
    system->purchaseItem(entry2);
    std::cout << system->to_string() << std::endl;
    double cost = system->sellItem(dbCode1, 6);
    std::cout << "COGS: " << cost << std::endl;
    std::cout << system->to_string() << std::endl;
    return 0;
}