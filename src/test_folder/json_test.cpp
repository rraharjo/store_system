#include <nlohmann/json.hpp>
#include <iostream>


void print_v(std::vector<std::string> &);
int main(int argc, char **argv)
{
    nlohmann::json my_json = nlohmann::json::parse(R"(
    {
      "main_command": 2,
      "items": [
        {
          "itemdb": "asd",
          "price": 10000,
          "qty": 10
        },
        {
          "itemdb": "dsd",
          "price": 450,
          "qty": 14
        }
      ]
    }
    )");
    int a = my_json["main_command"];
    std::vector<nlohmann::json> items = my_json["items"];
    try{
        std::string b = my_json.at("nonexistent");
    }
    catch (nlohmann::json_abi_v3_11_3::detail::out_of_range e){
        std::cout << " here" << std::endl;
        std::cout << e.what() << std::endl;
    }
    catch (std::exception e){
        std::cout << "generic" << std::endl;
        std::cout << e.what() << std::endl;
    }
    for (nlohmann::json &j : items){
        std::string db = j.at("itemdb");
        double price = j.at("price");
        int qty = j.at("qty");
        std::cout << db << " " << price << " " << qty << std::endl;
    }
    std::cout << a << std::endl;
    std::cout << my_json.dump() << std::endl;
}

void print_v(std::vector<std::string> &v){
    for (std::string s : v){
        std::cout << s << '\n';
    }
    std::cout << std::endl;
}