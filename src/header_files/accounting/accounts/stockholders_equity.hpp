#include <iostream>
#include "./account.hpp"
#include "../enums/stockholders_t_accounts.hpp"

#ifndef STOCKHOLDERSEQUITYACCOUNT_HPP
#define STOCKHOLDERSEQUITYACCOUNT_HPP
namespace accounting
{

    class StockholdersEquityAccount : public Account
    {
    public:
        StockholdersEquityAccount();

        void initiateTAccount();
    };
};
#endif