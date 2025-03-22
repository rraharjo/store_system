#include <iostream>
#include "header_files/accounting/accounting_system.hpp"
#include "util/factory/transaction_factory.hpp"

/*int main()
{
    accounting::AccountingSystem *a_system = accounting::AccountingSystem::get_instance();
    double buy_item = 199.99;
    double paidInCash = 100.00;
    double paidInCredit = 99.99;
    accounting::Transaction *buyItemTransaction = util::factory::GoodsPurchaseFactory("Buy item 1", buy_item, paidInCash, paidInCredit).create_transaction();
    a_system->add_transaction(buyItemTransaction);
    double sell_item = 250.00;
    double soldInCash = 200.00;
    double soldInCredit = 50.00;
    accounting::Transaction *sellGoods = util::factory::GoodsSellingFactory("sell item 1", sell_item, soldInCash, soldInCredit).create_transaction();
    a_system->add_transaction(sellGoods);
    accounting::Transaction *getCOGS = util::factory::GoodsSoldCOGSFactory("get item 1 cogs", buy_item).create_transaction();
    a_system->add_transaction(getCOGS);
    double employeeWages = 30.00;
    accounting::Transaction *payTheEmployee = util::factory::EmployeeWagesFactory("Pay day", employeeWages).create_transaction();
    a_system->add_transaction(payTheEmployee);
    double excavatorPrice = 1000.00;
    double payCashExc = 1000.00;
    accounting::Transaction *buyExcavator = util::factory::BuyEquipmentFactory("buy excavator", excavatorPrice, payCashExc, 0).create_transaction();
    a_system->add_transaction(buyExcavator);
    double depreciation_amount = 175.50; 
    accounting::Transaction *depreciateExcavator = util::factory::ApplyDepreciationFactory("Excavator depreciation", depreciation_amount).create_transaction();
    a_system->add_transaction(depreciateExcavator);
    double excavatorSellingPriceCash = 900.00;
    accounting::Transaction *sellingExcavator = util::factory::SellEquipmentFactory("Sell Excavator", depreciation_amount, excavatorPrice, excavatorSellingPriceCash, 0).create_transaction();
    a_system->add_transaction(sellingExcavator);
    std::cout << a_system->to_string() << std::endl;
    std::cout << a_system->assets->get_total_debit() - a_system->assets->get_total_credit() << std::endl;
    std::cout << a_system->liabilities->get_total_credit() - a_system->liabilities->get_total_debit() << std::endl;
    std::cout << a_system->stockholders_equity->get_total_credit() - a_system->stockholders_equity->get_total_debit() << std::endl;
    delete a_system; 
    delete buyItemTransaction;
    delete sellGoods;
    delete getCOGS;
    delete payTheEmployee;
    delete buyExcavator;
    delete sellingExcavator;
    return 0;
}*/

