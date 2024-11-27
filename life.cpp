#include "dict.hpp"
#include "io.hpp"
#include <unistd.h>

dict field;
var cols;
var rows;

var neighbours(const var &i, const var &j) {
    var count = 0;

    var icurr = i;
    const var& jcurr = j;

    var iprev = (icurr > 0 ? icurr : rows) - 1;
    var inext = icurr < rows - 1 ? icurr + 1 : 0;
    var jprev = (jcurr > 0 ? jcurr : cols) - 1;
    var jnext = jcurr < cols - 1 ? jcurr + 1 : 0;

    iprev *= cols;
    icurr *= cols;
    inext *= cols;

    count += field[iprev + jprev];
    count += field[iprev + jcurr];
    count += field[iprev + jnext];

    count += field[icurr + jprev];
    count += field[icurr + jnext];

    count += field[inext + jprev];
    count += field[inext + jcurr];
    count += field[inext + jnext];

    return count;
};

void display() {
    io::print("+" + "-" * cols + "+");
    for (var i = 0; i < rows; i++) {
        var line = "|";
        for (var j = 0; j < cols; j++)
            line += field[i * cols + j] ? "*" : " ";
        io::print(line + "|");
    }
    io::print("+" + "-" * cols + "+");
    io::print({"\x1b[" + (rows + 2) + "A"}, "");
    usleep(10000);
};

dict next() {
    dict tmp;
    tmp.resize(cols * rows);
    for (var i = 0; i < rows; i++) {
        for (var j = 0; j < cols; j++) {
            var n = neighbours(i, j);
            tmp[i * cols + j] = +(n == 3 || (field[i * cols + j] && n == 2));
        }
    }
    return tmp;
}

int main() {
    dict lines = io::readlines();
    rows = lines.size();

    for (var i = 0; i < rows; i++) {
        dict line = lines[i].split("");
        cols = cols < line.size() ? line.size() : cols;

        for (var j = 0; j < cols; j++)
            field[i * cols + j] = +(line[j] == "*");
    }

    int q = 0;
    while (q++ < 1000) {
        field = next();
        // display();
    }
}
