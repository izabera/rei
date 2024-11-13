#include "dict.hpp"
#include "io.hpp"
#include "var.hpp"

// fake 2d grid with grid[x][y] == grid[x*height+y]
// origin is at 0,0 in a grid that can span from -1M to 1M
var height = 2'000'000;

dict lay(dict &grid, const dict &wire, const var &sign) {
    dict dx, dy;
    dx["R"] = 1;
    dy["R"] = 0;

    dx["L"] = -1;
    dy["L"] = 0;

    dx["U"] = 0;
    dy["U"] = -1;

    dx["D"] = 0;
    dy["D"] = 1;

    var x = 0, y = 0, count = 0;

    var mindist = height * height;
    var mincount = height * height;

    auto abs = [](const var &v) { return v < 0 ? -v : +v; };

    for (auto [_, v] : wire) {
        var dir = v[0];
        var steps = +v[1, 10];
        for (var i = 0; i < steps; i++) {
            count += sign;
            x += dx[dir];
            y += dy[dir];

            auto &cell = grid[x * height + y];
            if (!cell)
                cell = count;
            else {
                if (cell * sign < 0) { // the sign is the same for self-intersections
                    auto currdist = abs(x) + abs(y);
                    if (mindist > currdist)
                        mindist = currdist;

                    auto currcount = abs(count) + abs(cell);
                    if (mincount > currcount)
                        mincount = currcount;
                }
            }
        }
    }

    dict ret;
    ret["part1"] = mindist;
    ret["part2"] = mincount;
    return ret;
}

int main() {
    auto wire1 = io::readline().split(",");
    auto wire2 = io::readline().split(",");

    dict grid;

    lay(grid, wire1, 1);
    auto tuple = lay(grid, wire2, -1);

    io::print({tuple["part1"], tuple["part2"]});
}
