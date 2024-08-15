#include "accounting/accounting_transaction/entry.hpp"
using namespace accounting;

Entry::Entry(bool debit, double amount, util::enums::AccountTitles account, int tAccount)
{
    this->debit = debit;
    this->amount = amount;
    this->account = account;
    this->tAccount = tAccount;
}

bool Entry::isDebit()
{
    return this->debit;
}

double Entry::getAmount()
{
    return this->amount;
}

int Entry::getTAccount()
{
    return this->tAccount;
}

std::string Entry::getTransactionTitle()
{
    return this->transactionTitle;
}

void Entry::setTransactionTitle(std::string title)
{
    this->transactionTitle = title;
}

long long Entry::getTransactionRef()
{
    return this->transactionRef;
}

void Entry::setTransactionRef(long long &reference)
{
    this->transactionRef = reference;
}

util::enums::AccountTitles Entry::getAccountTitle()
{
    return this->account;
}

std::string Entry::getTAccountName()
{
    switch (this->account)
    {
    case util::enums::AccountTitles::ASSETS:
        return util::enums::assetTAccountNames[static_cast<util::enums::AssetTAccount>(this->tAccount)];
    case util::enums::AccountTitles::LIABILITIES:
        return util::enums::liabilitiesTAccountNames[static_cast<util::enums::LiabilitiesTAccount>(this->tAccount)];
    case util::enums::AccountTitles::STOCKHOLDERSEQUITY:
        return util::enums::stockholdersTAccountNames[static_cast<util::enums::StockholdersTAccount>(this->tAccount)];
    }
    return NULL;
}

std::string Entry::to_string()
{
    std::string toRet = "";
    if (this->isDebit())
    {
        toRet += "\tD " + this->getTransactionTitle() + " " + std::to_string(this->amount);
    }
    else
    {
        toRet += "\tC " + this->getTransactionTitle() + " " + std::to_string(this->amount);
    }
    toRet += "\n";
    return toRet;
}