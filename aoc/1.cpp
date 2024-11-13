#include "io.hpp"
int main() {
    var part1, part2;

    var mass;
    while ((mass = io::readline())) {
        var fuel = (mass / 3 | 0) - 2;
        part1 += fuel;
        while (fuel > 0) {
            part2 += fuel;
            fuel = (fuel / 3 | 0) - 2;
        }
    }
    io::print({part1, part2});
}
