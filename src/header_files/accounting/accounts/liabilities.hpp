#include <iostream>
#include "./account.hpp"
#include "util/enums/accounting/liabilities_t_accounts.hpp"

#ifndef LIABILITIESACCOUNT_HPP
#define LIABILITIESACCOUNT_HPP

namespace accounting
{

    class Liabilities : public Account
    {
    public:
        Liabilities();
        
        void initiateTAccount() override;
    };
};
#endif