#include "store/store_system.hpp"

int main(int argc, char** argv){
    store::StoreSystem *sSystem = store::StoreSystem::get_instance();
    util::Date *trans1Date = new util::Date();
    store::PurchaseTransaction *trans1 = new store::PurchaseTransaction("seller1", trans1Date);
    inventory::Inventory *sellable1 = new inventory::Inventory("Hand Drill", "HD001", 100);
    inventory::Inventory *sellable2 = new inventory::Inventory("Electric Drill", "ED001", 249.99);
    sSystem->add_item(sellable1);
    sSystem->add_item(sellable2);
    inventory::PurchaseEntry* entry11 = new inventory::PurchaseEntry(sellable1->get_db_code(), trans1->get_db_code(), 80, 50);
    inventory::PurchaseEntry* entry12 = new inventory::PurchaseEntry(sellable2->get_db_code(), trans1->get_db_code(), 200, 50);
    trans1->add_entry(entry11);
    trans1->add_entry(entry12);
    sSystem->buy_item(trans1);
    //std::cout << sSystem->to_string() << std::endl;
    store::SellingTransaction *trans2 = new store::SellingTransaction(new util::Date());
    inventory::SellingEntry* entry21 = new inventory::SellingEntry(sellable1->get_db_code(), trans2->get_db_code(), sellable1->get_selling_price(), 30);
    inventory::SellingEntry* entry22 = new inventory::SellingEntry(sellable2->get_db_code(), trans2->get_db_code(), sellable2->get_selling_price(), 10);
    trans2->add_entry(entry21);
    trans2->add_entry(entry22);
    sSystem->sell_item(trans2);
    //std::cout << sSystem->to_string() << std::endl;
    util::Date *buyCar = new util::Date();
    util::Date *buyBuilding = new util::Date();
    inventory::Equipment *car = new inventory::Equipment("car", "", 100, 10, buyCar);
    car->insert_to_db();
    inventory::Equipment *building = new inventory::Equipment("Building", "", 1000, 30, buyBuilding);
    building->insert_to_db();
    sSystem->add_property(car);
    sSystem->add_property(building);
    store::PurchaseTransaction *purchaseCar = new store::PurchaseTransaction("", buyCar);
    inventory::PurchaseEntry *purchaseCarEntry = new inventory::PurchaseEntry(car->get_db_code(), purchaseCar->get_db_code(), 2500, 1);
    purchaseCar->add_entry(purchaseCarEntry);
    store::PurchaseTransaction *purchaseBuilding = new store::PurchaseTransaction("", buyBuilding);
    inventory::PurchaseEntry *purchaseBuildingEntry = new inventory::PurchaseEntry(building->get_db_code(), purchaseBuilding->get_db_code(), 500000, 1);
    purchaseBuilding->add_entry(purchaseBuildingEntry);
    sSystem->capitalize_asset(purchaseCar);
    sSystem->capitalize_asset(purchaseBuilding);
    //std::cout << sSystem->to_string() << '\n';
    util::Date *sellCarDate = new util::Date();
    store::SellingTransaction *sellCar = new store::SellingTransaction(sellCarDate);
    inventory::SellingEntry *sellCarEntry = new inventory::SellingEntry(car->get_db_code(), sellCar->get_db_code(), 2600, 1);
    sellCar->add_entry(sellCarEntry);
    sSystem->dispose_asset(sellCar);
    std::cout << sSystem->to_string() << '\n';
    std::cout << "process completed" << '\n';
    return 0;
}