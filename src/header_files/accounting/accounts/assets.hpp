#include <iostream>
#include "./account.hpp"
#include "util/enums/accounting/assets_t_accounts.hpp"

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