#include "dict.hpp"
#include "functional.hpp"
#include "io.hpp"

var compute(dict code, const var &val1, const var &val2) {
    code[1] = val1;
    code[2] = val2;

    for (var pos = 0; code[pos] != 99; pos += 4) {
        var lhs = code[code[pos + 1]];
        var rhs = code[code[pos + 2]];

        if (code[pos] == 1)
            code[code[pos + 3]] = lhs + rhs;
        else
            code[code[pos + 3]] = lhs * rhs;
    }
    return code[0];
}

int main() {
    dict code = io::readline().split(",").map(fn::tonum);

    var part1 = compute(code, 12, 2);
    var part2;

    for (var i = 0; i <= 99; i++) {
        for (var j = 0; j <= 99; j++) {
            var ret = compute(code, i, j);
            if (ret == 19690720) {
                part2 = i * 100 + j;
                break;
            }
        }
    }

    io::print({part1, part2});
}
