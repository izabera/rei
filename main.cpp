#include "dict.hpp"
#include "io.hpp"
#include "var.hpp"

using io::print, io::println;

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
        io::file f("file");
        println(f, "hello");
        auto other = f;
        auto other2 = f;
        auto other3 = static_cast<io::file &&>(f);
        println(other3, "world");
    }

    y = "left";
    y += "right";
    println(y);

    println(var{4} + 3);
    println(var{"4"} + 3);
    println(var{4} - 3);
    println(var{"4"} - 3);

    println("foo");
    println(io::err, {"writing", "to", "fd", 2});

    for (var x; x < 10; x++)
        println(x);

    println(var{.2} + .1);

    dict d;
    println(d.contains("2"));
    d[2] = "asdf";
    println(d[2]);
    println(d.contains("2")); // 2 becomes "2" during lookup
    println(d[2] == d["2"]);
    println(d[3]);
    d[4] = true;
    d["foo"] = "bar";
    println(d[4]);
    println(d["foo"]);

    for (auto [k, v] : d) {
        println({k, v});
    }
}
