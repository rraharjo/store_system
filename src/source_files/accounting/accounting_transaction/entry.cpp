#include <iostream>
#include <string>
#include "accounting/accounting_transaction/entry.hpp"
using namespace accounting;

Entry::Entry(bool debit, double amount, enums::AccountTitles account, int tAccount)
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

enums::AccountTitles Entry::getAccountTitle()
{
    return this->account;
}

std::string Entry::getTAccountName()
{
    switch (this->account)
    {
    case enums::AccountTitles::ASSETS:
        return enums::assetTAccountNames[static_cast<enums::AssetTAccount>(this->tAccount)];
    case enums::AccountTitles::LIABILITIES:
        return enums::liabilitiesTAccountNames[static_cast<enums::LiabilitiesTAccount>(this->tAccount)];
    case enums::AccountTitles::STOCKHOLDERSEQUITY:
        return enums::stockholdersTAccountNames[static_cast<enums::StockholdersTAccount>(this->tAccount)];
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