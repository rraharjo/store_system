#include "store/store_system.hpp"

int main(int argc, char** argv){
    store::StoreSystem *sSystem = new store::StoreSystem();
    store::PurchaseTransaction *trans1 = new store::PurchaseTransaction("seller1", new util::Date());
    inventory::Inventory *sellable1 = new inventory::Inventory("Hand Drill", "HD001", 100);
    inventory::Inventory *sellable2 = new inventory::Inventory("Electric Drill", "ED001", 249.99);
    sSystem->addItem(sellable1);
    sSystem->addItem(sellable2);
    inventory::PurchaseEntry* entry11 = new inventory::PurchaseEntry(sellable1->getDBCode(), trans1->getDBCode(), 80, 50);
    inventory::PurchaseEntry* entry12 = new inventory::PurchaseEntry(sellable2->getDBCode(), trans1->getDBCode(), 200, 50);
    trans1->addEntry(entry11);
    trans1->addEntry(entry12);
    sSystem->buyItem(trans1);
    std::cout << sSystem->toString() << std::endl;
    store::SellingTransaction *trans2 = new store::SellingTransaction(new util::Date());
    inventory::SellingEntry* entry21 = new inventory::SellingEntry(sellable1->getDBCode(), trans2->getDBCode(), sellable1->getSellingPrice(), 30);
    inventory::SellingEntry* entry22 = new inventory::SellingEntry(sellable2->getDBCode(), trans2->getDBCode(), sellable2->getSellingPrice(), 10);
    trans2->addEntry(entry21);
    trans2->addEntry(entry22);
    sSystem->sellItem(trans2);
    std::cout << sSystem->toString() << std::endl;
    util::Date *buyCar = new util::Date();
    util::Date *buyBuilding = new util::Date();
    inventory::Equipment *car = new inventory::Equipment("car", "", 100, 10, buyCar);
    inventory::Equipment *building = new inventory::Equipment("Building", "", 1000, 30, buyBuilding);
    sSystem->addProperty(car);
    sSystem->addProperty(building);
    store::PurchaseTransaction *purchaseCar = new store::PurchaseTransaction("", buyCar);
    inventory::PurchaseEntry *purchaseCarEntry = new inventory::PurchaseEntry(car->getDBCode(), purchaseCar->getDBCode(), 2500, 1);
    purchaseCar->addEntry(purchaseCarEntry);
    store::PurchaseTransaction *purchaseBuilding = new store::PurchaseTransaction("", buyBuilding);
    inventory::PurchaseEntry *purchaseBuildingEntry = new inventory::PurchaseEntry(building->getDBCode(), purchaseBuilding->getDBCode(), 500000, 1);
    purchaseBuilding->addEntry(purchaseBuildingEntry);
    sSystem->buyAsset(purchaseCar);
    sSystem->buyAsset(purchaseBuilding);
    std::cout << sSystem->toString() << '\n';
    util::Date *sellCarDate = new util::Date();
    store::SellingTransaction *sellCar = new store::SellingTransaction(sellCarDate);
    inventory::SellingEntry *sellCarEntry = new inventory::SellingEntry(car->getDBCode(), sellCar->getDBCode(), 2600, 1);
    sellCar->addEntry(sellCarEntry);
    sSystem->disposeAsset(sellCar);
    std::cout << sSystem->toString() << '\n';
    std::cout << "process completed" << '\n';
    return 0;
}