#include "dict.hpp"
#include "io.hpp"
#include "var.hpp"

using io::print;

int main() {
    var x = 3;
    var y = "meow";
    print(x);
    print({x + "\n"}, var{});

    print({x, y});
    y = 3;
    y += x;
    print({x, y});

    print("writing to file");
    {
        io::file f("file.tmp", io::w);
        f.print("hello (from f)");
        auto copy = f;
        copy.print("world (from copy)");
        auto copy2 = f;
        copy2.print("world (from copy2)");
        auto moved = static_cast<io::file &&>(f);
        moved.print("world (from moved)");
    }

    print("reading line by line");
    {
        io::file f("file.tmp");
        for (var q = 0, line; (line = f.readline()); q++)
            print({"line", q, line});
    }
    print("reading 5 bytes");
    print(io::file("file.tmp").read(5));
    print("reading the full file");
    print({io::file("file.tmp").read()}, var{});

    y = "left";
    y += "right";
    print(y);

    print(var{4} + 3);
    print(var{"4"} + 3);
    print(var{4} - 3);
    print(var{"4"} - 3);

    print("foo");
    io::err.print({"writing", "to", "fd", 2});

    for (var x = 0; x < 10; x++)
        print(x);

    print(var{.2} + .1);

    dict d;
    print(d.contains("2"));
    d[2] = "asdf";
    print(d[2]);
    print(d.contains("2")); // 2 becomes "2" during lookup
    print(d[2] == d["2"]);
    print(d[3]);
    d[4] = true;
    d["foo"] = "bar";
    print(d[4]);
    print(d["foo"]);

    var q;
    for (auto [k, v] : d)
        print({q++, k, v});

    var letters = "abcdefg";
    var nums = 1234567890;
    print(letters[2]);
    print(letters[2, 4]);
    print(nums[2]);
    print(nums[2, 4]);

    print(var{});
    var empty;
    print(empty);
    print(empty == var{});
    print(empty == empty);
    print(empty + "x" == "nullx");
    print(empty + 1 == 1);
    print(++empty == 1);
    print(empty++ == 1);
    print(empty == 2);

    for (auto [k, v] : "foo bar baz   bat"_v.split())
        print({k, v});
    for (auto [k, v] : "foo"_v.split())
        print({k, v});
    for (auto [k, v] : "|foo|bar|baz|"_v.split("|"))
        print({k, v});
    // needs a space between _v and . because wtf parser
    for (auto [k, v] : 123040506_v .split(0))
        print({k, v});

    var foo = "    leading and trailing spaces     ";
    print("x" + foo.strip() + "x");
    print("x" + foo.strip(" ls") + "x");
    print("x" + foo.strip("wxzy") + "x");
    print("x" + "unlawful"_v.strip({}) + "x");

    var numbers = "0123456789";
    print(numbers);
    print(numbers[0,4] + "x"_v * 6);
    print(numbers * 3);

    print("hello world");
    print("hello world", {1, 2, 3});
    print("hello world", {1, 2, 3}, "enddddd\n");
    print("foo <{}> bar <{}> baz", {1, 2, 3, 4});
    print("<{}> foo <{}> bar <{}> baz <{}>", {1, 2, 3, 4});
    print("<{}> foo <{}> <{}> <{}> baz <{}>", {1, 2, 3, 4});
}
