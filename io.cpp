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

file::file(const var &name) {
    var str = name + "";
    std::string &strname = *static_cast<std::string *>(str.str);
    impl = new std::shared_ptr<FILE>(fopen(strname.data(), "w+"), fclose);
}

struct console {
    console() {
        in.impl = new std::shared_ptr<FILE>(stdin, fclose);
        out.impl = new std::shared_ptr<FILE>(stdout, fclose);
        err.impl = new std::shared_ptr<FILE>(stderr, fclose);
    }
    ~console() {}
};
file err;
file out;
file in;

console cons;

void print(file &stream, const var &v) {
    auto file = Underlying(stream).get();
    switch (v.type) {
        case var::null:    fprintf(file, "null"); break;
        case var::boolean: fprintf(file, v.b ? "true" : "false"); break;
        case var::number:  fprintf(file, "%g", v.num); break;
        case var::string:  fprintf(file, "%s", static_cast<std::string *>(v.str)->data());
    }
}
void print(file &stream, std::initializer_list<var> vars) {
    var sep = "";
    for (const auto &v : vars) {
        print(stream, sep);
        sep = " ";
        print(stream, v);
    }
}
void print(const var &v) { print(out, v); }
void print(std::initializer_list<var> vars) { print(out, vars); }

void println(file &stream, const var &v) { print(stream, v + "\n"); }
void println(file &stream, std::initializer_list<var> vars) {
    print(stream, vars);
    print(stream, "\n");
}
void println(const var &v) { println(out, v); }
void println(std::initializer_list<var> vars) { println(out, vars); }

} // namespace io
