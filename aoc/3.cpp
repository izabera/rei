#include "dict.hpp"
#include "io.hpp"
#include "var.hpp"

// fake 2d grid with grid[x][y] == grid[x*height+y]
// origin is at 0,0 in a grid that can span from -1M to 1M
var height = 2'000'000;

var lay(dict &grid, const dict &wire, const var &wireid) {
    dict dx, dy;
    dx["R"] = 1;
    dy["R"] = 0;

    dx["L"] = -1;
    dy["L"] = 0;

    dx["U"] = 0;
    dy["U"] = -1;

    dx["D"] = 0;
    dy["D"] = 1;

    var x = 0, y = 0;

    var mindist = height;

    auto abs = [](const var &v) { return v < 0 ? -v : v; };

    for (auto [_, v] : wire) {
        var dir = v[0];
        var steps = +v[1, 10];
        for (var i = 0; i < steps; i++) {
            x += dx[dir];
            y += dy[dir];
            auto &cell = grid[x * height + y];
            if (cell && cell != wireid) {
                auto curr = abs(x) + abs(y);
                if (mindist > curr)
                    mindist = curr;
            }
            cell = wireid;
        }
    }
    return mindist;
}

int main() {
    dict wire1 = io::readline().split(",");
    dict wire2 = io::readline().split(",");

    dict grid;

    lay(grid, wire1, 1);
    io::println(lay(grid, wire2, 2));
}
