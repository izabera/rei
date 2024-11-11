#include "dict.hpp"
#include "io.hpp"

int main() {
    auto range = io::readline().split("-");
    auto lo = range[0];
    auto hi = range[1];

    // dumbest possible way to solve this
    var part1 = 0, part2 = 0;
    for (; lo <= hi; lo++) {
        var last = "";
        var hasdouble = false;
        var nondecreasing = true;
        var groupsok = false;
        var groupsize = 1;

        for (auto [_, d] : lo.split("")) {
            if (last > d) {
                nondecreasing = false;
                break;
            }
            if (last == d) {
                hasdouble = true;
                groupsize++;
            }
            else {
                if (groupsize == 2)
                    groupsok = true;
                groupsize = 1;
            }
            last = d;
        }
        if (nondecreasing && hasdouble) {
            part1++;
            if (groupsize == 2)
                groupsok = true;
            if (groupsok)
                part2++;
        }
    }
    io::println({part1, part2});
}
