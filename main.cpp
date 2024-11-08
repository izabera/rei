#include "io.hpp"
#include "var.hpp"

using Io::print, Io::println;

int main() {
    var x = 3;
    var y = "meow";
    print(x);
    print(x + "\n");
    println({x, y});

    y = 3;
    y += x;
    print({x, y, "\n"});

    {
        Io::File f("file");
        println(f, "hello");
    }

    y = "left";
    y += "right";
    println(y);

    println(var{4} + 3);
    println(var{"4"} + 3);
    println(var{4} - 3);
    println(var{"4"} - 3);

    println("foo");
    println(Io::err, {"writing", "to", "fd", 2});

    for (var x; x < 10; x++)
        println(x);
}
