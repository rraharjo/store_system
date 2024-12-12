#include "accounting/accounts/t_account.hpp"
using namespace accounting;

util::Table *TAccount::classTable = util::TAccountTable::getInstance();

void TAccount::initiateTAccountOnDB()
{
    std::vector<TAccount *> tAccountCollections;
    for (util::enums::TAccounts current = FIRST_TACCOUNT; current != LAST_TACCOUNT; ++current)
    {
        TAccount *newTAccount = new TAccount(current);
        newTAccount->insertToDB();
        tAccountCollections.push_back(newTAccount);
    }
    for (TAccount *tAccount : tAccountCollections)
    {
        delete tAccount;
    }
}

TAccount *TAccount::generateFromDatabase(util::enums::TAccounts tAccount)
{
    std::vector<util::TableCondition> conditions;
    std::vector<std::vector<std::string>> records;
    util::TableCondition matchingTitle = util::TableCondition();
    matchingTitle.col = util::enums::tAccountTableColumns[util::enums::TAccountTable::TITLE];
    matchingTitle.comparator = util::TableComparator::EQUAL;
    matchingTitle.value = util::enums::tAccountsNameMap[tAccount];
    conditions.push_back(matchingTitle);
    records = TAccount::classTable->getRecords(conditions);
    return new TAccount(tAccount, std::stod(records[0][1]), std::stod(records[0][2]));
}

std::vector<std::string> TAccount::getInsertParameter()
{
    std::vector<std::string> toRet;
    toRet.push_back(util::enums::tAccountsNameMap[this->title]);
    toRet.push_back(std::to_string(this->debitAmount));
    toRet.push_back(std::to_string(this->creditAmount));
    return toRet;
}

std::vector<std::string> TAccount::getUpdateParameter()
{
    std::vector<std::string> toRet;
    toRet.push_back(this->getDBCode());
    toRet.push_back(std::to_string(this->debitAmount));
    toRet.push_back(std::to_string(this->creditAmount));
    return toRet;
}

void TAccount::insertToDB()
{
    this->insertToDBWithTable(TAccount::classTable, false);
};

void TAccount::updateToDB()
{
    this->updateToDBWithTable(TAccount::classTable);
};

TAccount::TAccount(util::enums::TAccounts title, double debit, double credit)
    : util::baseclass::HasTable()
{
    this->title = title;
    this->debitAmount = debit;
    this->creditAmount = credit;
    this->setDBCode(util::enums::tAccountsNameMap[this->title]);
    this->debitEntries = std::vector<Entry *>();
    this->creditEntries = std::vector<Entry *>();
}

TAccount::TAccount(util::enums::TAccounts title)
    : TAccount(title, 0, 0)
{
}

TAccount::~TAccount()
{
    for (Entry *e : this->debitEntries)
    {
        delete e;
    }
    for (Entry *e : this->creditEntries)
    {
        delete e;
    }
}

util::enums::TAccounts TAccount::getTitle()
{
    return this->title;
}

std::string TAccount::getTitleName()
{
    return util::enums::tAccountsNameMap[this->title];
}

void TAccount::addEntry(Entry *entry)
{
    if (entry->isDebit())
    {
        this->debitEntries.push_back(entry);
        this->debitAmount += entry->getAmount();
    }
    else
    {
        this->creditEntries.push_back(entry);
        this->creditAmount += entry->getAmount();
    }
    this->updateToDB();
}

double TAccount::getDebitAmount()
{
    double total = 0;
    for (Entry *event : this->debitEntries)
    {
        total += event->getAmount();
    }
    return total;
}

double TAccount::getCreditAmount()
{
    double total = 0;
    for (Entry *event : this->creditEntries)
    {
        total += event->getAmount();
    }
    return total;
}

double TAccount::getTotalAmount()
{
    return this->getDebitAmount() - this->getCreditAmount();
}

std::string TAccount::to_string()
{
    std::string toRet = this->getTitleName() + "\n";
    for (Entry *entry : this->debitEntries)
    {
        toRet += entry->to_string();
    }
    for (Entry *entry : this->creditEntries)
    {
        toRet += entry->to_string();
    }
    return toRet;
}