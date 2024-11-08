#include "io.hpp"
#include <cstdio>
#include <memory>
#include <string>
#include <utility>

#define File(f) (static_cast<std::shared_ptr<FILE> *>((f).impl)->get())

io &io::operator=(io &&other) {
    if (this != &other)
        std::swap(this->impl, other.impl);
    return *this;
}
io &io::operator=(const io &other) {
    if (this != &other) {
        io tmp = other;
        std::swap(this->impl, tmp.impl);
    }
    return *this;
}

io::~io() { delete static_cast<std::shared_ptr<FILE> *>(impl); }

io::io(const var &name) {
    var str = name + "";
    std::string &strname = *static_cast<std::string *>(str.str);
    impl = new std::shared_ptr<FILE>(fopen(strname.data(), "w+"), fclose);
}

io &io::operator<<(const var &v) {
    if (v.type == var::number)
        fprintf(File(*this), "%g", v.num);
    else
        fprintf(File(*this), "%s", static_cast<std::string *>(v.str)->data());
    return *this;
}

io &io::operator>>(var &) { return *this; }

void io::print(var v, io& stream) {
    if (v.type == var::number)
        fprintf(File(stream), "%g", v.num);
    else
        fprintf(File(stream), "%s", static_cast<std::string *>(v.str)->data());
}
void io::print(std::initializer_list<var> vars, io& stream) {
    for (auto v : vars)
        print(v, stream);
}
void io::println(var v, io& stream) {
    print(v, stream);
    print("\n", stream);
}
void io::println(std::initializer_list<var> vars, io& stream) {
    for (auto v : vars)
        println(v, stream);
}

struct console {
    console() {
        io::in.impl = new std::shared_ptr<FILE>(stdin, fclose);
        io::out.impl = new std::shared_ptr<FILE>(stdout, fclose);
        io::err.impl = new std::shared_ptr<FILE>(stderr, fclose);
    }
    ~console() {}
};
io io::err;
io io::out;
io io::in;
static console cons;
