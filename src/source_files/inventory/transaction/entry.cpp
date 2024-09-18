#include "inventory/transaction/entry.hpp"
using namespace inventory;
Entry::Entry(std::string itemDBCode, std::string transactionCode, double price, int qty)
{
    if (!itemDBCode.compare(0, 3, "SEL")){
        this->sellableDBCode = itemDBCode;
        this->propertiesDBCode = "";
    }
    else{
        this->propertiesDBCode = itemDBCode;
        this->sellableDBCode = "";
    }
    this->transactionDBCode = transactionCode;
    this->price = price;
    this->qty = qty;
}

std::string Entry::getSellableDBCode()
{
    return this->sellableDBCode;
}

std::string Entry::getPropertiesDBCode(){
    return this->propertiesDBCode;
}

std::string Entry::getTransactionDBCode()
{
    return this->transactionDBCode;
}

double Entry::getPrice()
{
    return this->price;
}

int Entry::getQty()
{
    return this->qty;
}

util::Date *Entry::getTransactionDate()
{
    return this->transactionDate;
}

void Entry::setTransactionDate(util::Date *transactionDate)
{
    this->transactionDate = transactionDate;
}

/******************************************************************************/


int PurchaseEntry::nextItemCode = 0; //TO DO: change to count(*)
void PurchaseEntry::setTable()
{
    this->table = util::PurchaseEntryTable::getInstance();
}

std::vector<std::string> PurchaseEntry::getInsertParameter()
{
    std::vector<std::string> args;
    if (this->getDBCode() == ""){
        args.push_back(this->createDBCode());
    }
    else{
        args.push_back(this->getDBCode());
    }
    args.push_back(this->getSellableDBCode() == "" ? "NULL" : this->getSellableDBCode());
    args.push_back(this->getPropertiesDBCode() == "" ? "NULL" : this->getPropertiesDBCode());
    args.push_back(this->getTransactionDBCode());
    args.push_back(std::to_string(this->getPrice()));
    args.push_back(std::to_string(this->getQty()));
    args.push_back(std::to_string(this->getAvailableQty()));
    return args;
}

std::string PurchaseEntry::createDBCode(){
    char numAsString[6];
    sprintf(numAsString, "%05d", PurchaseEntry::nextItemCode++);
    std::string countAsString = numAsString;
    std::string dbCode = "PNT" + countAsString;
    return dbCode;
}

PurchaseEntry::PurchaseEntry(std::string sellableDBCode, std::string transactionCode, double price, int qty) : Entry(sellableDBCode, transactionCode, price, qty)
{
    this->setTable();
    this->availableQty = qty;
}
int PurchaseEntry::getAvailableQty()
{
    return this->availableQty;
}
void PurchaseEntry::setAvailableQty(int qty)
{
    this->availableQty = qty;
}

/************************************************************************/
int SellingEntry::nextItemCode = 0;//TO DO: change to count(*)

void SellingEntry::setTable()
{
    this->table = util::SellingEntryTable::getInstance();
}

std::vector<std::string> SellingEntry::getInsertParameter()
{
    std::vector<std::string> args;
    if (this->getDBCode() == ""){
        args.push_back(this->createDBCode());
    }
    else{
        args.push_back(this->getDBCode());
    }
    args.push_back(this->getSellableDBCode() == "" ? "NULL" : this->getSellableDBCode());
    args.push_back(this->getPropertiesDBCode() == "" ? "NULL" : this->getPropertiesDBCode());
    args.push_back(this->getTransactionDBCode());
    args.push_back(std::to_string(this->getPrice()));
    args.push_back(std::to_string(this->getQty()));
    return args;
}

std::string SellingEntry::createDBCode(){
    char numAsString[6];
    sprintf(numAsString, "%05d", SellingEntry::nextItemCode++);
    std::string countAsString = numAsString;
    std::string dbCode = "SNT" + countAsString;
    return dbCode;
}

SellingEntry::SellingEntry(std::string sellableDBCode, std::string transactionCode, double price, int qty) : Entry(sellableDBCode, transactionCode, price, qty)
{
    this->setTable();
}