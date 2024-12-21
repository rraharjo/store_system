#include "driver/driver.hpp"

int main(int argc, char **argv){
    storedriver::StdDriver my_driver = storedriver::StdDriver();
    my_driver.start();
    return 0;
}