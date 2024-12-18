#include "accounting/accounting_transaction/entry.hpp"
using namespace accounting;

util::Table *Entry::classTable = util::AccountingEntryTable::getInstance();

std::vector<Entry *> Entry::generateFromDB(std::string transactionCode)
{
    std::vector<Entry *> toRet;
    std::vector<util::TableCondition> conditions;
    util::TableCondition cond1 = util::TableCondition();
    cond1.col = util::enums::accountingEntryTableColumns[util::enums::AccountingEntryTable::ATDBCODE];
    cond1.comparator = util::TableComparator::EQUAL;
    cond1.value = transactionCode;
    conditions.push_back(cond1);
    std::vector<std::vector<std::string>> records = classTable->getRecords(conditions);
    for (std::vector<std::string> &record : records)
    {
        Entry *newEntry = new Entry(record[0], record[1], record[2] == "t" ? true : false, std::stod(record[3]),
                                    util::enums::getTAccountEnum(record[4]));
        toRet.push_back(newEntry);
    }
    return toRet;
}

void Entry::insertToDB()
{
    this->insertToDBWithTable(Entry::classTable);
};

void Entry::updateToDB()
{
    this->updateToDBWithTable(Entry::classTable);
};

std::vector<std::string> Entry::getInsertParameter()
{
    std::vector<std::string> toRet;
    toRet.push_back(util::enums::primaryKeyCodesMap[util::enums::PrimaryKeyCodes::ACCOUNTINGENTRY]);
    toRet.push_back(this->transactionDB);
    toRet.push_back(this->debit ? "true" : "false");
    toRet.push_back(std::to_string(this->amount));
    toRet.push_back(util::enums::tAccountsNameMap[this->tAccount]);
    toRet.push_back(util::enums::accountTitlesMap[this->account]);
    return toRet;
}

std::vector<std::string> Entry::getUpdateParameter()
{
    std::vector<std::string> toRet;
    toRet.push_back(this->transactionDB);
    toRet.push_back(this->debit ? "true" : "false");
    toRet.push_back(std::to_string(this->amount));
    toRet.push_back(util::enums::tAccountsNameMap[this->tAccount]);
    toRet.push_back(util::enums::accountTitlesMap[this->account]);
    return toRet;
}

Entry::Entry(std::string dbCode, std::string transactionDBCode, bool debit, double amount,
             util::enums::TAccounts tAccount)
    : util::baseclass::HasTable()
{
    this->setDBCode(dbCode);
    this->debit = debit;
    this->amount = amount;
    this->tAccount = tAccount;
    this->account = util::enums::getAccountTitle(this->tAccount);
    this->transactionTitle = "";
    this->transactionDB = transactionDBCode;
}

Entry::Entry(std::string transactionDBCode, bool debit, double amount,
             util::enums::TAccounts tAccount)
    : Entry::Entry("", transactionDBCode, debit, amount, tAccount)
{
}

bool Entry::isDebit()
{
    return this->debit;
}

double Entry::getAmount()
{
    return this->amount;
}

std::string Entry::getTransactionDB()
{
    return this->transactionDB;
}

std::string Entry::getTransactionTitle()
{
    return this->transactionTitle;
}

util::enums::TAccounts Entry::getTAccount()
{
    return this->tAccount;
}

util::enums::AccountTitles Entry::getAccountTitle()
{
    return this->account;
}

std::string Entry::getTAccountName()
{
    return util::enums::getName(this->tAccount);
}

std::string Entry::getAccountTitleName()
{
    return util::enums::getName(this->account);
}

void Entry::setTransactionDB(std::string transactionDB)
{
    this->transactionDB = transactionDB;
}

void Entry::setTransactionTitle(std::string title)
{
    this->transactionTitle = title;
}

std::string Entry::to_string()
{
    std::string toRet = "";
    if (this->isDebit())
    {
        toRet += this->getDBCode() + " D \"" + this->getTransactionTitle() + "\" " + std::to_string(this->amount);
    }
    else
    {
        toRet += this->getDBCode() + " C \"" + this->getTransactionTitle() + "\" " + std::to_string(this->amount);
    }
    toRet += "\n";
    return toRet;
}