#include "accounting/accounting_transaction/entry.hpp"
using namespace accounting;

util::Table *Entry::classTable = util::AccountingEntryTable::getInstance();

void Entry::insertToDB(){
    this->insertToDBWithTable(Entry::classTable);
};

void Entry::updateToDB(){
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

Entry::Entry(bool debit, double amount, util::enums::AccountTitles account, util::enums::TAccounts tAccount) : util::baseclass::HasTable()
{
    this->debit = debit;
    this->amount = amount;
    this->account = account;
    this->tAccount = tAccount;
    this->transactionTitle = "";
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
        toRet += "\tD \"" + this->getTransactionTitle() + "\" " + std::to_string(this->amount);
    }
    else
    {
        toRet += "\tC \"" + this->getTransactionTitle() + "\" " + std::to_string(this->amount);
    }
    toRet += "\n";
    return toRet;
}