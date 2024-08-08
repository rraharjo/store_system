#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include "util/date.hpp"
#include "./entry.hpp"

#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP
namespace accounting
{
    class Transaction
    {
    private:
        std::vector<Entry *> debitEntries;
        std::vector<Entry *> creditEntries;
        std::string name;
        long long timeStamp;

    public:
        Transaction(std::string name);

        long long getTimestamp();

        void addEntry(Entry *entry);

        double getDebitAmount();

        double getCreditAmount();

        bool isBalanced();

        std::vector<Entry *> &getDebitEntries();

        std::vector<Entry *> &getCreditEntries();
    };
};
#endif