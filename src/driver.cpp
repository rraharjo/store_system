#include <iostream>
#include <limits>
#include "store/store_system.hpp"

#define OPT_SIZE 8
std::string menu();
int getInt(std::string, int, int);
double getDouble(std::string, double, double);
std::string getString(std::string);
util::Date *getDate(std::string);
void addNewInventory(store::StoreSystem *);
void purchaseInventory(store::StoreSystem *);
void addPurchaseEntry(store::PurchaseTransaction *);
void purchaseAsset(store::StoreSystem *);
void capitalizeAssets(store::StoreSystem *);
void sellInventory(store::StoreSystem *);
void sellAssets(store::StoreSystem *);
void listOfItems(store::StoreSystem *);

int main(int argc, char *argv[])
{
    store::StoreSystem *mySystem = store::StoreSystem::getInstance();
    int option = -1;
    while (option)
    {
        std::cout << menu();
        option = getInt("Enter your choice:", 0, OPT_SIZE - 1);
        switch (option)
        {
        case 1:
            addNewInventory(mySystem);
            break;
        case 2:
            purchaseInventory(mySystem);
            break;
        case 3:
            purchaseAsset(mySystem);
            break;
        case 4:
            capitalizeAssets(mySystem);
            break;
        case 5:
            sellInventory(mySystem);
            break;
        case 6:
            sellAssets(mySystem);
            break;
        case 7:
            listOfItems(mySystem);
            break;
        default:
            break;
        }
    }
}

std::string menu()
{
    std::string toRet = "0. exit\n";
    toRet += "1. Add new inventory\n";
    toRet += "2. Purchase inventory\n";
    toRet += "3. Purchase asset\n";
    toRet += "4. Capitalize asset\n";
    toRet += "5. Sell inventory\n";
    toRet += "6. Sell Asset\n";
    toRet += "7. Get a list of item\n";
    return toRet;
}

int getInt(std::string message, int intMin, int intMax)
{
    int option = -1;
    std::cout << message << '\n';
    std::cout << "Enter option between " << intMin << " and " << intMax << '\n';
    while (1)
    {
        std::cin >> option;
        if (option >= intMin && option <= intMax)
        {
            break;
        }
        std::cout << "Invalid option\n";
    }
    return option;
}

double getDouble(std::string message, double dMin, double dMax)
{
    double option;
    std::cout << message << '\n';
    std::cout << "Enter double between " << dMin << " and " << dMax << '\n';
    while (1)
    {
        std::cin >> option;
        if (option >= dMin && option <= dMax)
        {
            break;
        }
        std::cout << "Invalid option\n";
    }
    return option;
}

std::string getString(std::string message)
{
    std::string toRet = "";
    std::cout << message << '\n';
    std::cin >> toRet;
    return toRet;
}

util::Date *getDate(std::string message)
{
    std::cout << "format: dd/MM/yyyy\n";
    std::string format = getString(message);
    util::Date *toRet = new util::Date(format);
    return toRet;
}

void addNewInventory(store::StoreSystem *sSystem)
{
    std::string name = getString("Enter product name:");
    std::string itemCode = getString("Enter item code:");
    double price = getDouble("Enter price:", 0.0, std::numeric_limits<double>::max());
    inventory::Inventory *newInventory = new inventory::Inventory(itemCode, name, price);
    newInventory->insertToDB();
    sSystem->addItem(newInventory);
    std::cout << "new inventory added" << '\n';
}

void purchaseInventory(store::StoreSystem *sSystem)
{
    std::string seller = getString("Enter transaction seller:");
    util::Date *transDate = getDate("Enter date: ");
    store::PurchaseTransaction *newTransaction = new store::PurchaseTransaction(seller, transDate);
    addPurchaseEntry(newTransaction);
    int more = getInt("more?", 0, 1);
    while (more)
    {
        addPurchaseEntry(newTransaction);
        more = getInt("more?", 0, 1);
    }
    double paidCash = getDouble("How much cash paid? ", 0, newTransaction->getTransactionAmount());
    newTransaction->setPaidCash(paidCash);
    newTransaction->setPaidCredit(newTransaction->getTransactionAmount() - paidCash);
    newTransaction->insertToDB();
    sSystem->buyItem(newTransaction);
    return;
}

void addPurchaseEntry(store::PurchaseTransaction *p)
{
    std::string itemCode = getString("Enter item DB Code:");
    double price = getDouble("Enter each item price", 0.0, std::numeric_limits<double>::max());
    int qty = getInt("Enter qty purchased: ", 0, INT_MAX);
    inventory::PurchaseEntry *newEntry = new inventory::PurchaseEntry(itemCode, p->getDBCode(), price, qty);
    p->addEntry(newEntry);
}

void purchaseAsset(store::StoreSystem *sSystem)
{
    std::string name = getString("Name: ");
    std::string itemCode = getString("item code: ");
    double purchaseCost = getDouble("Purchase cost: ", 0.0, std::numeric_limits<double>::max());
    double residualValue = getDouble("Residual value: ", 0.0, std::numeric_limits<double>::max());
    int usefulLife = getInt("Year useful life: ", 1, 99);
    util::Date *datePurchased = getDate("date purchased ");
    inventory::Equipment *newEqp = new inventory::Equipment(name, itemCode, residualValue, usefulLife, datePurchased);
    store::PurchaseTransaction *newTransaction = new store::PurchaseTransaction("", datePurchased);
    inventory::PurchaseEntry *newEntry = new inventory::PurchaseEntry(newEqp->getDBCode(), "", purchaseCost, 1);
    newTransaction->addEntry(newEntry);
    double paidCash = getDouble("How much cash paid? ", 0, newTransaction->getTransactionAmount());
    newTransaction->setPaidCash(paidCash);
    newTransaction->setPaidCredit(newTransaction->getTransactionAmount() - paidCash);
    newEqp->insertToDB();
    newTransaction->insertToDB();
    sSystem->addProperty(newEqp);
    sSystem->capitalizeAsset(newTransaction);
}

void capitalizeAssets(store::StoreSystem *sSystem)
{
    std::string itemCode = getString("item code: ");
    double capitalizedAmt = getDouble("Capitalized amount: ", 0.0, std::numeric_limits<double>::max());
    util::Date *transDate = getDate("transaction date: ");
    store::PurchaseTransaction *newTransaction = new store::PurchaseTransaction("", transDate);
    inventory::PurchaseEntry *newEntry = new inventory::PurchaseEntry(itemCode, newTransaction->getDBCode(), capitalizedAmt, 1);
    newTransaction->addEntry(newEntry);
    double paidCash = getDouble("How much cash paid? ", 0, newTransaction->getTransactionAmount());
    newTransaction->setPaidCash(paidCash);
    newTransaction->setPaidCredit(newTransaction->getTransactionAmount() - paidCash);
    newTransaction->insertToDB();
    sSystem->capitalizeAsset(newTransaction);
}

void sellInventory(store::StoreSystem *sSystem)
{
    std::string itemCode = getString("inventory item code: ");
    int qty = getInt("qty: ", 1, INT_MAX);
    double price = sSystem->getInventory(itemCode)->getSellingPrice();
    util::Date *date = getDate("enter date: ");
    store::SellingTransaction *newTransaction = new store::SellingTransaction(date);
    inventory::SellingEntry *newEntry = new inventory::SellingEntry(itemCode, newTransaction->getDBCode(), price, qty);
    newTransaction->addEntry(newEntry);
    int more = getInt("more?", 0, 1);
    while (more)
    {
        itemCode = getString("inventory item code: ");
        qty = getInt("qty: ", 1, INT_MAX);
        price = sSystem->getInventory(itemCode)->getSellingPrice();
        newEntry = new inventory::SellingEntry(itemCode, newTransaction->getDBCode(), price, qty);
        newTransaction->addEntry(newEntry);
        more = getInt("more?", 0, 1);
    }
    double paidCash = getDouble("How much cash paid?", 0, newTransaction->getTransactionAmount());
    newTransaction->setPaidCash(paidCash);
    newTransaction->setPaidCredit(newTransaction->getTransactionAmount() - paidCash);
    newTransaction->insertToDB();
    sSystem->sellItem(newTransaction);
}

void sellAssets(store::StoreSystem *sSystem)
{
    std::string itemCode = getString("asset item code: ");
    double price = getDouble("selling price", 0, std::numeric_limits<double>::max());
    util::Date *date = getDate("enter date: ");
    store::SellingTransaction *newTransaction = new store::SellingTransaction(date);
    
    inventory::SellingEntry *newEntry = new inventory::SellingEntry(itemCode, newTransaction->getDBCode(), price, 1);
    newTransaction->addEntry(newEntry);
    double paidCash = getDouble("How much cash paid?", 0, newTransaction->getTransactionAmount());
    newTransaction->setPaidCash(paidCash);
    newTransaction->setPaidCredit(newTransaction->getTransactionAmount() - paidCash);
    newTransaction->insertToDB();
    sSystem->disposeAsset(newTransaction);
}

void listOfItems(store::StoreSystem *sSystem)
{
    std::cout << sSystem->toStringInv() << std::endl;
}