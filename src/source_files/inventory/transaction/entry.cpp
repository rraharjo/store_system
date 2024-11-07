#include "inventory/transaction/entry.hpp"
using namespace inventory;
Entry::Entry(std::string itemDBCode, std::string transactionCode, double price, int qty)
{
    if (!itemDBCode.compare(0, 3, util::enums::primaryKeyCodesMap[util::enums::PrimaryKeyCodes::INVENTORY]))
    {
        this->sellableDBCode = itemDBCode;
        this->propertiesDBCode = "";
    }
    else
    {
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

std::string Entry::getPropertiesDBCode()
{
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

void Entry::setTransactionDBCode(std::string dbCode){
    this->transactionDBCode = dbCode;
}

/******************************************************************************/

std::vector<util::TableCondition> getInventoryCondition(std::string invDBCode)
{
    std::vector<util::TableCondition> conditions;
    util::TableCondition cond1 = util::TableCondition(), cond2 = util::TableCondition();
    cond1.col = util::enums::purchaseEntryTableColumns[util::enums::PurchaseEntryTable::INVENTORYDBCODE];
    cond1.comparator = util::TableComparator::EQUAL;
    cond1.value = invDBCode;
    cond2.col = util::enums::purchaseEntryTableColumns[util::enums::PurchaseEntryTable::AVAILABLEQTY];
    cond2.comparator = util::TableComparator::MORETHAN;
    cond2.value = "0";
    conditions.push_back(cond1);
    conditions.push_back(cond2);
    return conditions;
}

std::vector<util::TableCondition> getEquipmentCondition(std::string eqpDBCode)
{
    std::vector<util::TableCondition> conditions;
    util::TableCondition cond1 = util::TableCondition();
    cond1.col = util::enums::purchaseEntryTableColumns[util::enums::PurchaseEntryTable::ASSETSCODE];
    cond1.comparator = util::TableComparator::EQUAL;
    cond1.value = eqpDBCode;
    conditions.push_back(cond1);
    return conditions;
}

util::Table *PurchaseEntry::classTable = util::PurchaseEntryTable::getInstance();

std::vector<PurchaseEntry *> PurchaseEntry::generateFromDatabase(std::string itemDBCode)
{
    std::vector<PurchaseEntry *> toRet;
    std::vector<util::TableCondition> conditions;
    std::vector<std::string> columns;
    std::vector<std::vector<std::string>> records;
    size_t itemIndex;
    for (auto it = util::enums::purchaseEntryTableColumns.begin(); it != util::enums::purchaseEntryTableColumns.end(); it++)
    {
        columns.push_back(it->second.columnName);
    }
    if (itemDBCode.compare(0, 3, "INV") == 0){
        conditions = getInventoryCondition(itemDBCode);
        itemIndex = 1;
    }
    else{
        conditions = getEquipmentCondition(itemDBCode);
        itemIndex = 2;
    }
    records = PurchaseEntry::classTable->getRecords(columns, conditions);
    for (std::vector<std::string> &record : records)
    {
        PurchaseEntry *newEntry = new PurchaseEntry(record[0], record[itemIndex], record[3],
                                                    std::stod(record[4]), std::stoi(record[5]), std::stoi(record[6]));
        toRet.push_back(newEntry);
    }
    return toRet;
}

std::vector<std::string> PurchaseEntry::getInsertParameter()
{
    std::vector<std::string> args;
    args.push_back(util::enums::primaryKeyCodesMap[util::enums::PrimaryKeyCodes::PURCHASEENTRY]);
    args.push_back(this->getSellableDBCode() == "" ? "NULL" : this->getSellableDBCode());
    args.push_back(this->getPropertiesDBCode() == "" ? "NULL" : this->getPropertiesDBCode());
    args.push_back(this->getTransactionDBCode());
    args.push_back(std::to_string(this->getPrice()));
    args.push_back(std::to_string(this->getQty()));
    args.push_back(std::to_string(this->getAvailableQty()));
    return args;
}

std::vector<std::string> PurchaseEntry::getUpdateParameter()
{
    std::vector<std::string> args;
    args.push_back(this->getSellableDBCode() == "" ? "NULL" : this->getSellableDBCode());
    args.push_back(this->getPropertiesDBCode() == "" ? "NULL" : this->getPropertiesDBCode());
    args.push_back(this->getTransactionDBCode());
    args.push_back(std::to_string(this->getPrice()));
    args.push_back(std::to_string(this->getQty()));
    args.push_back(std::to_string(this->getAvailableQty()));
    return args;
}

void PurchaseEntry::insertToDB()
{
    this->insertToDBWithTable(PurchaseEntry::classTable);
}

void PurchaseEntry::updateToDB()
{
    this->updateToDBWithTable(PurchaseEntry::classTable);
}

PurchaseEntry::PurchaseEntry(std::string dbCode, std::string invDBCode, std::string transactionDBCode,
                             double price, int allQty, int availableQty)
    : Entry(invDBCode, transactionDBCode, price, allQty)
{
    this->setDBCode(dbCode);
    this->availableQty = availableQty;
}

PurchaseEntry::PurchaseEntry(std::string invDBCode, std::string transactionDBCode, double price, int qty)
    : PurchaseEntry("", invDBCode, transactionDBCode, price, qty, qty)
{
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
util::Table *SellingEntry::classTable = util::SellingEntryTable::getInstance();
int SellingEntry::nextItemCode = 0; 

std::vector<std::string> SellingEntry::getInsertParameter()
{
    std::vector<std::string> args;
    args.push_back(util::enums::primaryKeyCodesMap[util::enums::PrimaryKeyCodes::SELLINGENTRY]);
    args.push_back(this->getSellableDBCode() == "" ? "NULL" : this->getSellableDBCode());
    args.push_back(this->getPropertiesDBCode() == "" ? "NULL" : this->getPropertiesDBCode());
    args.push_back(this->getTransactionDBCode());
    args.push_back(std::to_string(this->getPrice()));
    args.push_back(std::to_string(this->getQty()));
    return args;
}

std::vector<std::string> SellingEntry::getUpdateParameter()
{
    std::vector<std::string> args;
    args.push_back(this->getSellableDBCode() == "" ? "NULL" : this->getSellableDBCode());
    args.push_back(this->getPropertiesDBCode() == "" ? "NULL" : this->getPropertiesDBCode());
    args.push_back(this->getTransactionDBCode());
    args.push_back(std::to_string(this->getPrice()));
    args.push_back(std::to_string(this->getQty()));
    return args;
}

void SellingEntry::insertToDB()
{
    this->insertToDBWithTable(SellingEntry::classTable);
}

void SellingEntry::updateToDB()
{
    this->updateToDBWithTable(SellingEntry::classTable);
}

SellingEntry::SellingEntry(std::string sellableDBCode, std::string transactionCode, double price, int qty) : Entry(sellableDBCode, transactionCode, price, qty)
{
}