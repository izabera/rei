#include "var.hpp"

#include <iostream>
int main() {
    var x = 3;
    var y = "meow";
    std::cout << x.num << '\n';
    std::cout << *static_cast<std::string*>(y.str) << '\n';
    y = 3;
    std::cout << y.num << '\n';
}
