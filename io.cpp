#include "io.hpp"
#include "var.hpp"
#include <cstdio>
#include <memory>
#include <string>

#define Underlying(f) (*static_cast<std::shared_ptr<FILE> *>((f).impl))
namespace io {

file::file(const file &other) { impl = new std::shared_ptr<FILE>(Underlying(other)); }
file::file(file &&other) { std::swap(this->impl, other.impl); }
file &file::operator=(const file &other) {
    if (this != &other) {
        file tmp{other};
        std::swap(this->impl, tmp.impl);
    }
    return *this;
}
file &file::operator=(file &&other) {
    if (this != &other)
        std::swap(this->impl, other.impl);
    return *this;
}

file::~file() { delete static_cast<std::shared_ptr<FILE> *>(impl); }

file::file(const var &name, mode m) {
    var str = name + "";
    std::string &strname = *static_cast<std::string *>(str.u.str);

    const char *fopenmode = m == r ? "r" : m == w ? "w" : "w+";
    auto ptr = fopen(strname.data(), fopenmode);
    impl = new std::shared_ptr<FILE>(ptr, fclose);
}

struct console {
    console() {
        in.impl = new std::shared_ptr<FILE>(stdin, fclose);
        out.impl = new std::shared_ptr<FILE>(stdout, fclose);
        err.impl = new std::shared_ptr<FILE>(stderr, fclose);
    }
};
file err;
file out;
file in;

console cons;

void file::print(const var &v) {
    auto file = Underlying(*this).get();
    auto var = v + "";
    fprintf(file, "%s", static_cast<std::string *>(var.u.str)->data());
}
void file::print(std::initializer_list<var> vars) {
    auto i = 0;
    for (const auto &v : vars) {
        if (i++)
            print(" ");
        print(v);
    }
}
void file::println(const var &v) { print(v + "\n"); }
void file::println(std::initializer_list<var> vars) {
    print(vars);
    print("\n");
}

void print(const var &v) { out.print(v); }
void print(std::initializer_list<var> vars) { out.print(vars); }
void println(const var &v) { out.println(v); }
void println(std::initializer_list<var> vars) { out.println(vars); }

var readline() { return in.readline(); }
var read(const var &v) { return in.read(v); }

var file::readline() {
    var ret;
    auto file = Underlying(*this).get();

    char *line = nullptr;
    size_t size = 0;
    long l = getline(&line, &size, file);
    if (l != -1) {
        if (line[l - 1] == '\n')
            line[--l] = 0;

        ret.type = var::string;
        ret.u.str = new std::string(line, size);
    }

    free(line);
    return ret;
}

var file::read(const var &len) {
    var ret;
    auto file = Underlying(*this).get();

    auto remaining = len.type == var::null ? LONG_MAX : long((+len).u.num);
    std::string tmp;

    constexpr long bufsize = 1024;
    static char buf[bufsize];

    while (1) {
        auto want = std::min(remaining, bufsize);
        long l = fread(buf, 1, want, file);
        if (l <= 0)
            break;
        remaining -= l;
        tmp += {buf, size_t(l)};
    }

    ret.type = var::string;
    ret.u.str = new std::string(tmp);

    return ret;
}
} // namespace io
