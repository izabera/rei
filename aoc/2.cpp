#include "dict.hpp"
#include "functional.hpp"
#include "io.hpp"
int main() {
    dict code = io::readline().split(",").map(fn::tonum);

    code[1] = 12;
    code[2] = 2;
    for (var pos = 0; code[pos] != 99; pos += 4) {
        var lhs = code[code[pos + 1]];
        var rhs = code[code[pos + 2]];

        if (code[pos] == 1)
            code[code[pos + 3]] = lhs + rhs;
        else
            code[code[pos + 3]] = lhs * rhs;
    }
    io::println(code[0]);
}
