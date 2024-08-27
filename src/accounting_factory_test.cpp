#include <iostream>
#include "header_files/accounting/accounting_system.hpp"
#include "util/factory/transaction_factory.hpp"

int main()
{
    accounting::AccountingSystem *aSystem = accounting::AccountingSystem::getInstance();
    double buyItem = 199.99;
    double paidInCash = 100.00;
    double paidInCredit = 99.99;
    accounting::Transaction *buyItemTransaction = util::factory::GoodsPurchaseFactory("Buy item 1", buyItem, paidInCash, paidInCredit).createTransaction();
    aSystem->addTransaction(buyItemTransaction);
    double sellItem = 250.00;
    double soldInCash = 200.00;
    double soldInCredit = 50.00;
    accounting::Transaction *sellGoods = util::factory::GoodsSellingFactory("sell item 1", sellItem, soldInCash, soldInCredit).createTransaction();
    aSystem->addTransaction(sellGoods);
    accounting::Transaction *getCOGS = util::factory::GoodsSoldCOGSFactory("get item 1 cogs", buyItem).createTransaction();
    aSystem->addTransaction(getCOGS);
    double employeeWages = 30.00;
    accounting::Transaction *payTheEmployee = util::factory::EmployeeWagesFactory("Pay day", employeeWages).createTransaction();
    aSystem->addTransaction(payTheEmployee);
    double excavatorPrice = 1000.00;
    double payCashExc = 1000.00;
    accounting::Transaction *buyExcavator = util::factory::BuyEquipmentFactory("buy excavator", excavatorPrice, payCashExc, 0).createTransaction();
    aSystem->addTransaction(buyExcavator);
    double depreciationAmount = 175.50; 
    accounting::Transaction *depreciateExcavator = util::factory::ApplyDepreciationFactory("Excavator depreciation", depreciationAmount).createTransaction();
    aSystem->addTransaction(depreciateExcavator);
    double excavatorSellingPriceCash = 900.00;
    accounting::Transaction *sellingExcavator = util::factory::SellEquipmentFactory("Sell Excavator", depreciationAmount, excavatorPrice, excavatorSellingPriceCash, 0).createTransaction();
    aSystem->addTransaction(sellingExcavator);
    std::cout << aSystem->to_string() << std::endl;
    delete aSystem; 
    delete buyItemTransaction;
    delete sellGoods;
    delete getCOGS;
    delete payTheEmployee;
    delete buyExcavator;
    delete sellingExcavator;
    return 0;
}

