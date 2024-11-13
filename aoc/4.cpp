#include "dict.hpp"
#include "io.hpp"

int main() {
    auto range = io::readline().split("-");
    auto lo = +range[0];
    auto hi = +range[1];

    var part1 = 0, part2 = 0;
    for (; lo <= hi; lo++) {
        var last = "";
        var hasdouble = false;
        var groupsok = false;
        var groupsize = 1;

        for (auto [pos, d] : lo.split("")) {
            if (last > d) {
                // the minimum next candidate is xxxlastlastlast
                lo = lo[0, pos] + last * (var{6} - pos);
                // decrement by 1 so the for can reincrement...
                lo--;
                goto next;
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
        if (hasdouble) {
            part1++;
            if (groupsize == 2)
                groupsok = true;
            if (groupsok)
                part2++;
        }
    next:;
    }
    io::println({part1, part2});
}
