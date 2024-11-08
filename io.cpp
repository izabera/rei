#include "io.hpp"
#include <cstdio>
#include <memory>
#include <string>

#define Underlying(f) (*static_cast<std::shared_ptr<FILE> *>((f).impl))
namespace Io {

File::File(const File &other) { impl = new std::shared_ptr<FILE>(Underlying(other)); }
File::File(File &&other) { std::swap(this->impl, other.impl); }
File &File::operator=(const File &other) {
    if (this != &other) {
        File tmp{other};
        std::swap(this->impl, tmp.impl);
    }
    return *this;
}
File &File::operator=(File &&other) {
    if (this != &other)
        std::swap(this->impl, other.impl);
    return *this;
}

File::~File() { delete static_cast<std::shared_ptr<FILE> *>(impl); }

File::File(const var &name) {
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
File err;
File out;
File in;

console cons;

void print(File &stream, const var &v) {
    if (v.type == var::number)
        fprintf(Underlying(stream).get(), "%g", v.num);
    else
        fprintf(Underlying(stream).get(), "%s", static_cast<std::string *>(v.str)->data());
}
void print(File &stream, std::initializer_list<var> vars) {
    var sep = "";
    for (const auto &v : vars) {
        print(stream, sep);
        sep = " ";
        print(stream, v);
    }
}
void print(const var &v) { print(out, v); }
void print(std::initializer_list<var> vars) { print(out, vars); }

void println(File &stream, const var &v) { print(stream, v + "\n"); }
void println(File &stream, std::initializer_list<var> vars) {
    print(stream, vars);
    print(stream, "\n");
}
void println(const var &v) { println(out, v); }
void println(std::initializer_list<var> vars) { println(out, vars); }

} // namespace Io
