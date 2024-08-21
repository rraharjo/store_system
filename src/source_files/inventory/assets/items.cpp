#include "inventory/assets/items.hpp"

using namespace inventory;

Item::Item(std::string name = "", std::string itemCode = "")
{
    this->name = name;
    this->itemCode = itemCode;
}

std::string Item::getName(){
    return this->name;
}

std::string Item::getItemCode(){
    return this->itemCode;
}