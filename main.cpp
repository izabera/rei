#include "io.hpp"
#include "var.hpp"

int main() {
    var x = 3;
    var y = "meow";
    io::out << x << " " << y << "\n";

    y = 3;
    y += x;
    io::out << y << "\n";

    {
        io f("file");
        f << "hello\n";
    }

    y = "left";
    y += "right";
    io::out << y << "\n";

    io::out << var{4} + 3 << "\n";
    io::out << var{"4"} + 3 << "\n";
    io::out << var{4} - 3 << "\n";
    io::out << var{"4"} - 3 << "\n";

    io::println("foo");
    io::println({"four", 4, "five", 5}, io::err);
}
