#include "inventory/assets/items.hpp"

using namespace inventory;

Item::Item(std::string name = "", std::string itemCode = "")
{
    this->name = name;
    this->itemCode = itemCode;
    this->purchaseHistory = new PurchaseHistory();
    this->sellingHistory = new SellingHistory();
}

std::string Item::getName()
{
    return this->name;
}

std::string Item::getItemCode()
{
    return this->itemCode;
}

int Item::getQty()
{
    return this->qty;
}
