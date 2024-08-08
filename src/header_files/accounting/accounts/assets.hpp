#include <iostream>
#include "./account.hpp"
#include "../enums/assets_t_accounts.hpp"

#ifndef ASSETSACCOUNT_HPP
#define ASSETSACCOUNT_HPP

namespace accounting
{

    class Assets : public Account
    {
    public:
        Assets();

        void initiateTAccount() override;
    };
};
#endif