#include "inventory/inventory_system.hpp"
using namespace inventory;
int main(){
    inventory::InventorySystem *system = InventorySystem::get_instance();
    std::string propPurchaseTransactionCode = "PTR00001";
    std::string propSellTranscationCode = "STR00001";
    Inventory *sellable1 = new Inventory("Drill battery", "DB123", 100.00);
    Inventory *sellable2 = new Inventory("JackHammer", "JH123", 120.00);
    std::string dbCode1 = sellable1->get_db_code();
    std::string dbCode2 = sellable2->get_db_code();
    system->add_new_item(sellable1);
    system->add_new_item(sellable2);
    PurchaseEntry *entry1 = new PurchaseEntry(dbCode1, propPurchaseTransactionCode, 90, 5);
    PurchaseEntry *entry2 = new PurchaseEntry(dbCode1, propPurchaseTransactionCode, 80, 5);
    std::cout << system->to_string() << std::endl;
    system->purchase_sellables(entry1);
    system->purchase_sellables(entry2);
    SellingEntry *sellEntry = new SellingEntry(dbCode1, propSellTranscationCode, sellable1->get_selling_price(), 6);
    std::cout << system->to_string() << std::endl;
    double cost = system->sell_sellables(sellEntry);
    std::cout << "COGS: " << cost << std::endl;
    std::cout << system->to_string() << std::endl;
    util::Date *buyCar = new util::Date();
    util::Date *buyBuilding = new util::Date();
    Equipment *car = new Equipment("Car", "", 200, 5, buyCar);
    Equipment *shop = new Equipment("Shop", "", 200, 10, buyBuilding);
    system->add_new_property(car);
    system->add_new_property(shop);
    PurchaseEntry *propEntry1 = new PurchaseEntry(car->get_db_code(), propPurchaseTransactionCode, 1000, 1);
    PurchaseEntry *propEntry2 = new PurchaseEntry(shop->get_db_code(), propPurchaseTransactionCode, 1000000, 1);
    system->purchase_properties(propEntry1);
    system->purchase_properties(propEntry2);
    std::cout << system->to_string() << std::endl;
    SellingEntry *disposeCar = new SellingEntry(car->get_db_code(), propSellTranscationCode, 1100, 1);
    system->sell_properties(disposeCar);
    std::cout << system->to_string() << std::endl;
    return 0;
}