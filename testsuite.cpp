#include "dict.hpp"
#include "io.hpp"
#include "var.hpp"
#include <print>
#include <set>
#include <string>
#include <unistd.h>
#include <vector>

struct test {
    bool verbose = false;
    int failures = 0;
    int successes = 0;
    // make this a set if needed
    std::set<int> tests;

    bool tty = ::isatty(1);
    const char *bad = tty ? "\x1b[31mFAIL\x1b[m" : "FAIL";
    const char *good = tty ? "\x1b[32m OK \x1b[m" : " OK ";

    void check_equal(const auto line, const auto &lexpr, const auto &rexpr, //
                     const auto &lval, const auto &rval) {

        if (!tests.empty() && !tests.contains(line))
            return;

        if (lval != rval) {
            io::print("[{}]:{} {} != {} (was: {} expected: {})",
                      {bad, line, lexpr, rexpr, lval, rval});
            failures++;
        }
        else {
            successes++;
            if (verbose)
                io::print("[{}]:{} {} == {} ({})", {good, line, lexpr, rexpr, lval});
        }
    };

    ~test() {
        io::print("[{}] pass: {} fail: {}", {failures > 0 ? bad : good, successes, failures});
    }
};

int main(int argc, char **argv) {
    test t;
    if (argc > 1) {
        std::vector<std::string> args(argv + 1, argv + argc);
        for (auto arg : args) {
            if (arg == "--verbose")
                t.verbose = true;
            else
                // this is dumb
                t.tests.insert(std::stoi(arg));
        }
    }

#define check_equal(x, y) t.check_equal(__LINE__, #x, #y, x, y)

    check_equal("meow"_v, "meow");

    check_equal(null == null, true);
    check_equal(null <= null, true);
    check_equal(null >= null, true);
    check_equal(null != null, false);

    check_equal(null + null, null);
    check_equal(null - null, null);
    check_equal(null * null, null);
    check_equal(null / null, null);
    check_equal(null % null, null);

    check_equal(null + 7, 7);
    check_equal(null - 7, -7);
    check_equal(null * 7, null);
    check_equal(null / 7, null);
    check_equal(null % 7, null);

    check_equal(7 + null, 7);
    check_equal(7 - null, 7);
    check_equal(7 * null, null);
    check_equal(7 / null, null);
    check_equal(7 % null, null);

    check_equal(7 < null, false);
    check_equal(7 > null, true);
    check_equal(7 <= null, false);
    check_equal(7 >= null, true);

    check_equal(7_v + 0, 7);
    check_equal(7_v - 0, 7);
    check_equal(7_v * 0, null);
    // divisions by 0 or null always return null
    check_equal(7_v / 0, null);
    check_equal(7_v % 0, null);

    check_equal(0_v + 0, 0);
    check_equal(0_v - 0, 0);
    check_equal(0_v * 0, 0);
    check_equal(0_v / 0, null);
    check_equal(0_v % 0, null);

    check_equal(7_v * true, 7);
    check_equal(7_v * false, null);
    check_equal(7_v + true, 8);
    check_equal(7_v + false, 7);

    // numbers become strings when compared with strings
    check_equal("123"_v, 123);
    check_equal("0123"_v != 123, true);
    check_equal("023"_v < 123, true);
    check_equal("23"_v > 123, true);
    // +str becomes a number
    check_equal(+"23"_v < 123, true);
    check_equal(+"1e4"_v, 10000);
    // kinda like in js
    check_equal("10"_v + 3, "103");
    check_equal("10"_v - 3, 7);
    check_equal("meow"_v + "meow", "meowmeow");
    check_equal("meow"_v + null, "meownull");
    check_equal("meow"_v + false, "meowfalse");
    check_equal("meow"_v * null, null);
    check_equal("meow"_v * false, null);
    check_equal(++3_v, 4);
    check_equal(++"3"_v, 4);
    // str * n = n copies of the string, kinda like in python
    check_equal("meow"_v * 3_v, "meowmeowmeow");
    check_equal(3_v * "meow"_v, "meowmeowmeow");

    auto nums = "0123456789"_v;
    check_equal(nums[3], 3);
    check_equal((nums[3, 3]), 345);
    check_equal((nums[3, {}]), 3456789);

    check_equal("foobar"_v.strip(), "foobar");
    check_equal("   foobar    "_v.strip(), "foobar");
    check_equal("foobar"_v.strip("fro"), "ba");

    io::file("file.tmp", io::w).print("meow");
    check_equal(io::file("file.tmp").readline(), "meow");

    {
        auto file = io::file("file.tmp", io::w);
        file.print("foo <{}> bar <{}> baz", {1, 2, 3, 4});
        file.print("<{}> foo <{}> bar <{}> baz <{}>", {1, 2, 3, 4});
        file.print("<{}> foo <{}> <{}> <{}> baz <{}>", {1, 2, 3, 4});
        file = io::file("file.tmp");
        check_equal(file.readline(), "foo <1> bar <2> baz");
        check_equal(file.readline(), "<1> foo <2> bar <3> baz <4>");
        check_equal(file.readline(), "<1> foo <2> <3> <4> baz <null>");
    }

    check_equal("123 456"_v.split()[1], 456);

    {
        auto file = io::file("file.tmp");
        check_equal(bool{file}, true);
        auto bytes = file.read(3);
        check_equal(bytes, "foo");
        auto lines = file.readlines();
        check_equal(lines.size(), 3);
        check_equal(bool{file}, false);
        file = io::file("/this/file/does/not/exist", io::w);
        check_equal(bool{file}, false);
    }

#if 0
    // test that leaksan is working
    new int;
#if 0
    // test that if we steal ownership of stderr, it's closed at the end of main
    // (so leaksan can't print a report)
    auto f = std::move(io::err);
#endif
#endif

    {
        auto file = io::file("file.tmp", io::w);
        file.print("foo\0bar"_v);
        file = io::file("file.tmp");
        auto bytes = file.readline();
        check_equal(bytes, "foo\0bar"_v);
        check_equal(bytes.len(), 7);
    }
}
