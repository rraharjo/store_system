#include "store/store_system.hpp"

int main(int argc, char** argv){
    store::StoreSystem *sSystem = new store::StoreSystem();
    inventory::Sellable *sellable1 = new inventory::Sellable("Hand Drill", "HD001", 100);
    inventory::Sellable *sellable2 = new inventory::Sellable("Electric Drill", "ED001", 249.99);
    sSystem->addItem(sellable1);
    sSystem->addItem(sellable2);
    store::PurchaseTransaction *trans1 = new store::PurchaseTransaction("seller1", new util::Date());
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
    return 0;
}