#include "inventory/inventory_system.hpp"
using namespace inventory;
int main(){
    inventory::InventorySystem *system = InventorySystem::getInstance();
    Sellable *sellable1 = new Sellable("Drill battery", "DB123", 100.00);
    Sellable *sellable2 = new Sellable("JackHammer", "JH123", 120.00);
    int dbCode1 = sellable1->getDBCode();
    int dbCode2 = sellable2->getDBCode();
    system->addNewItem(sellable1);
    system->addNewItem(sellable2);
    PurchaseEntry *entry1 = new PurchaseEntry(dbCode1, 1, 90, 5);
    PurchaseEntry *entry2 = new PurchaseEntry(dbCode1, 1, 80, 5);
    std::cout << system->to_string() << std::endl;
    system->purchaseItem(entry1);
    system->purchaseItem(entry2);
    SellingEntry *sellEntry = new SellingEntry(dbCode1, 1, sellable1->getSellingPrice(), 6);
    std::cout << system->to_string() << std::endl;
    double cost = system->sellItem(sellEntry);
    std::cout << "COGS: " << cost << std::endl;
    std::cout << system->to_string() << std::endl;
    return 0;
}