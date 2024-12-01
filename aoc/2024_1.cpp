#include "dict.hpp"
#include "functional.hpp"
#include "io.hpp"
#include "var.hpp"

int main() {
    var part1, part2;
    dict left, right, lookup;

    for (auto [i, line] : io::readlines()) {
        auto fields = line.split();
        left[i]  = +fields[0];
        right[i] = +fields[1];
        lookup[right[i]]++;
    }
    left.sort();
    right.sort();

    for (auto [i, l] : left) {
        part1 += fn::abs(l - right[i]);
        part2 += l * lookup[l];
    }
    io::print({part1, part2});
}
