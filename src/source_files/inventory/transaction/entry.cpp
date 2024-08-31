#include "inventory/transaction/entry.hpp"
using namespace inventory;
Entry::Entry(int sellableDBCode, int transactionCode, double price, int qty)
{
    this->sellableDBCode = sellableDBCode;
    this->transactionDBCode = transactionCode;
    this->price = price;
    this->qty = qty;
}

int Entry::getSellableDBCode()
{
    return this->sellableDBCode;
}

int Entry::getTransactionDBCode()
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

void PurchaseEntry::setTable()
{
    this->table = util::PurchaseEntryTable::getInstance();
}

std::vector<std::string> PurchaseEntry::getInsertParameter()
{
    std::vector<std::string> args;
    args.push_back(std::to_string(this->getSellableDBCode()));
    args.push_back(std::to_string(this->getTransactionDBCode()));
    args.push_back(std::to_string(this->getPrice()));
    args.push_back(std::to_string(this->getQty()));
    args.push_back(std::to_string(this->getAvailableQty()));
    return args;
}

PurchaseEntry::PurchaseEntry(int sellableDBCode, int transactionCode, double price, int qty) : Entry(sellableDBCode, transactionCode, price, qty)
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

void SellingEntry::setTable()
{
    this->table = util::SellingEntryTable::getInstance();
}

std::vector<std::string> SellingEntry::getInsertParameter()
{
    std::vector<std::string> args;
    args.push_back(std::to_string(this->getSellableDBCode()));
    args.push_back(std::to_string(this->getTransactionDBCode()));
    args.push_back(std::to_string(this->getPrice()));
    args.push_back(std::to_string(this->getQty()));
    return args;
}

SellingEntry::SellingEntry(int sellableDBCode, int transactionCode, double price, int qty) : Entry(sellableDBCode, transactionCode, price, qty)
{
    this->setTable();
}