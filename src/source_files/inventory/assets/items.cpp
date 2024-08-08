#include <string>
#include "inventory/assets/items.hpp"

using namespace inventory;

Item::Item(std::string name = "", std::string itemCode = "")
{
    this->name = name;
    this->itemCode = itemCode;
}
