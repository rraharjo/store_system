#include "accounting/accounting_transaction/transaction.hpp"
using namespace accounting;

util::Table *Transaction::classTable = util::AccountingTransactionTable::getInstance();

std::vector<Transaction *> Transaction::generateFromDatabase()
{
    util::Date now = util::Date();
    std::string stringDate = std::to_string(now.get_m_day()) + "/" +
                             std::to_string(now.get_month()) + "/" +
                             std::to_string(now.get_year() - 1);
    util::Date lastYear = util::Date(stringDate);
    std::vector<util::TableCondition> conditions;
    util::TableCondition cond1 = util::TableCondition();
    cond1.col = util::enums::accountingTransactionTableColumns[util::enums::AccountingTransactionTable::TRANSACTIONDATE];
    cond1.comparator = util::TableComparator::MORETHAN;
    cond1.value = lastYear.to_db_format();
    conditions.push_back(cond1);
    std::vector<Transaction *> toRet;
    std::vector<std::vector<std::string>> records = classTable->getRecords(conditions);
    for (std::vector<std::string> &record : records)
    {
        util::Date *transactionDate = new util::Date(record[2], "%Y-%m-%d");
        Transaction *transactionFromDB = new Transaction(record[0], record[1], transactionDate, record[3]);
        std::vector<Entry *> entries = Entry::generateFromDB(transactionFromDB->getDBCode());
        for (Entry *entry : entries){
            transactionFromDB->addEntry(entry);
        }
        toRet.push_back(transactionFromDB);
    }
    return toRet;
}

void Transaction::insertToDB()
{
    this->insertToDBWithTable(Transaction::classTable);
};

void Transaction::updateToDB()
{
    this->updateToDBWithTable(Transaction::classTable);
};

Transaction::Transaction(std::string dbCode, std::string name, util::Date *transactionDate, std::string pid)
    : util::baseclass::HasTable()
{
    this->setDBCode(dbCode);
    this->name = name;
    this->debitEntries = {};
    this->creditEntries = {};
    this->transactionDate = transactionDate;
    this->entityID = pid;
}

Transaction::Transaction(std::string name, util::Date *transactionDate, std::string pid)
    : util::baseclass::HasTable()
{
    this->name = name;
    this->debitEntries = {};
    this->creditEntries = {};
    this->transactionDate = transactionDate;
    this->entityID = pid;
}

Transaction::Transaction(std::string name, util::Date *transactionDate)
    : Transaction::Transaction(name, transactionDate, "")
{
}

Transaction::Transaction(std::string name, std::string pid)
    : Transaction::Transaction(name, new util::Date(), pid)
{
}

Transaction::Transaction(std::string name) : Transaction::Transaction(name, new util::Date())
{
}

Transaction::~Transaction()
{
    for (Entry *e : this->creditEntries)
    {
        if (e->getDBCode() != "")
        {
            delete e;
        }
    }

    for (Entry *e : this->debitEntries)
    {
        if (e->getDBCode() != "")
        {
            delete e;
        }
    }
}

std::vector<std::string> Transaction::getInsertParameter()
{
    std::vector<std::string> args;
    args.push_back(util::enums::primaryKeyCodesMap[util::enums::PrimaryKeyCodes::ACCOUNTINGTRANSACTION]);
    args.push_back(this->name);
    args.push_back(this->transactionDate->to_db_format());
    args.push_back(this->entityID == "" ? "NULL" : this->entityID);
    return args;
}

std::vector<std::string> Transaction::getUpdateParameter()
{
    std::vector<std::string> args;
    args.push_back(this->name);
    args.push_back(this->transactionDate->to_db_format());
    args.push_back(this->entityID == "" ? "NULL" : this->entityID);
    return args;
}

std::vector<Entry *> &Transaction::getDebitEntries()
{
    return this->debitEntries;
}

std::vector<Entry *> &Transaction::getCreditEntries()
{
    return this->creditEntries;
}

double Transaction::getDebitAmount()
{
    double total = 0;
    for (Entry *entry : this->debitEntries)
    {
        total += (*entry).getAmount();
    }
    return total;
}

double Transaction::getCreditAmount()
{
    double total = 0;
    for (Entry *entry : this->creditEntries)
    {
        total += (*entry).getAmount();
    }
    return total;
}

void Transaction::addEntry(Entry *entry)
{
    if (entry->isDebit())
    {
        this->debitEntries.push_back(entry);
    }
    else
    {
        this->creditEntries.push_back(entry);
    }
    entry->setTransactionDB(this->getDBCode());
    entry->setTransactionTitle(this->name);
}

bool Transaction::isBalanced()
{
    return this->getDebitAmount() == this->getCreditAmount();
}

std::string Transaction::toString(){
    std::string toRet = "";
    toRet += "database code: " + this->getDBCode() + "\n";
    toRet += "name: " + this->name + "\n";
    toRet += "date: " + this->transactionDate->to_string() + "\n";
    toRet += "related entity: " + this->entityID + "\n";
    toRet += "=========Entries=========\n";
    for (Entry *entry : this->debitEntries){
        toRet += entry->to_string();
    }
    for (Entry *entry : this->creditEntries){
        toRet += entry->to_string();
    }
    return toRet;
}
