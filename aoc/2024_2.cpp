#include "dict.hpp"
#include "functional.hpp"
#include "io.hpp"
#include "var.hpp"

int main() {
    var part1 = 0, part2 = 0;

    for (auto [i, line] : io::readlines()) {
        auto fields = line.split().map(fn::tonum);

        // quadratic because it was too annoying to do otherwise
        auto check = [](const dict &d) {
            auto sign = [](const auto &x) -> var { return x < 0 ? -1 : x > 0 ? 1 : 0; };
            var old;
            var direction;
            var q = 0;
            for (auto [i, elem] : d) {
                if (q > 0) {
                    auto diff = old - elem;
                    auto abs  = fn::abs(diff);
                    auto dir  = sign(diff);
                    if (q == 1)
                        direction = dir;
                    if (q >= 1) {
                        if (dir != direction || abs > 3) {
                            return false;
                        }
                    }
                }
                old = elem;
                q++;
            }
            return true;
        };
        if (check(fields)) {
            part1++;
            part2++;
            continue;
        }
        auto tmp = fields;
        for (auto [i, elem] : fields) {
            tmp.remove(i);
            if (check(tmp)) {
                part2++;
                goto after;
            }
            tmp[i] = elem;
        }
    after:;
    }
    io::print({part1, part2});
}
