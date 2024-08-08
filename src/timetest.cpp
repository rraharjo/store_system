#include "util/date.hpp"
#include <iostream>
int main(){
    util::Date *feb292020 = new util::Date("29/02/2020");
    util::Date *jan302021 = new util::Date("30/01/2021");
    util::Date *mar012021 = new util::Date("01/03/2021");
    util::Date *feb282021 = new util::Date("28/02/2021");
    util::Date *mar012020 = new util::Date("01/03/2020");
    util::Date *mar012022 = new util::Date("01/03/2022");

    std::cout << feb292020->to_string() << std::endl;
    std::cout << feb292020->diffDaysTo(jan302021) << std::endl;
    std::cout << feb292020->diffYearsTo(jan302021) << std::endl;
    std::cout << feb292020->diffDaysTo(mar012021) << std::endl;
    std::cout << feb292020->diffYearsTo(mar012021) << std::endl;
    std::cout << feb292020->diffDaysTo(feb282021) << std::endl;
    std::cout << feb292020->diffYearsTo(feb282021) << std::endl;
    std::cout << mar012020->diffDaysTo(mar012021) << std::endl;
    std::cout << mar012020->diffYearsTo(mar012021) << std::endl;
    std::cout << mar012021->diffDaysTo(mar012022) << std::endl;
    std::cout << mar012021->diffYearsTo(mar012022) << std::endl;
    return 0;
}