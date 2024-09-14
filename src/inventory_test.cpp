#include "inventory/inventory_system.hpp"
using namespace inventory;
int main(){
    inventory::InventorySystem *system = InventorySystem::getInstance();
    std::string propPurchaseTransactionCode = "PTR00001";
    std::string propSellTranscationCode = "STR00001";
    Sellable *sellable1 = new Sellable("Drill battery", "DB123", 100.00);
    Sellable *sellable2 = new Sellable("JackHammer", "JH123", 120.00);
    std::string dbCode1 = sellable1->getDBCode();
    std::string dbCode2 = sellable2->getDBCode();
    system->addNewItem(sellable1);
    system->addNewItem(sellable2);
    PurchaseEntry *entry1 = new PurchaseEntry(dbCode1, propPurchaseTransactionCode, 90, 5);
    PurchaseEntry *entry2 = new PurchaseEntry(dbCode1, propPurchaseTransactionCode, 80, 5);
    std::cout << system->to_string() << std::endl;
    system->purchaseSellables(entry1);
    system->purchaseSellables(entry2);
    SellingEntry *sellEntry = new SellingEntry(dbCode1, propSellTranscationCode, sellable1->getSellingPrice(), 6);
    std::cout << system->to_string() << std::endl;
    double cost = system->sellSellables(sellEntry);
    std::cout << "COGS: " << cost << std::endl;
    std::cout << system->to_string() << std::endl;
    util::Date *buyCar = new util::Date();
    util::Date *buyBuilding = new util::Date();
    Depreciable *car = new Depreciable("Car", "", 200, 5, buyCar);
    Depreciable *shop = new Depreciable("Shop", "", 200, 10, buyBuilding);
    system->addNewProperty(car);
    system->addNewProperty(shop);
    PurchaseEntry *propEntry1 = new PurchaseEntry(car->getDBCode(), propPurchaseTransactionCode, 1000, 1);
    PurchaseEntry *propEntry2 = new PurchaseEntry(shop->getDBCode(), propPurchaseTransactionCode, 1000000, 1);
    system->purchaseProperties(propEntry1);
    system->purchaseProperties(propEntry2);
    std::cout << system->to_string() << std::endl;
    SellingEntry *disposeCar = new SellingEntry(car->getDBCode(), propSellTranscationCode, 1100, 1);
    system->sellProperties(disposeCar);
    std::cout << system->to_string() << std::endl;
    return 0;
}