#include "io.hpp"
#include "dict.hpp"
#include "var.hpp"
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

template <typename t1, typename t2>
using same_const_t = std::conditional_t<std::is_const_v<std::remove_reference_t<t1>>, const t2, t2>;

#define Str(v) (*reinterpret_cast<same_const_t<decltype(v), std::string> *>(&(v).u.buf))
#define Underlying(f) (*static_cast<std::shared_ptr<FILE> *>((f).impl))

namespace io {

file::file(const file &other) { impl = new std::shared_ptr<FILE>(Underlying(other)); }
file::file(file &&other) { std::swap(impl, other.impl); }
file &file::operator=(const file &other) {
    if (this != &other) {
        file tmp{other};
        std::swap(impl, tmp.impl);
    }
    return *this;
}
file &file::operator=(file &&other) {
    if (this != &other)
        std::swap(impl, other.impl);
    return *this;
}

file::~file() { delete static_cast<std::shared_ptr<FILE> *>(impl); }

file::file(const var &name, mode m) {
    var str = name + "";
    auto &strname = Str(str);

    const char *fopenmode = m == r ? "r" : m == w ? "w" : "w+";
    auto ptr = fopen(strname.data(), fopenmode);
    if (ptr)
        impl = new std::shared_ptr<FILE>(ptr, fclose);
}

file::operator bool() const {
    if (impl) {
        auto ptr = static_cast<std::shared_ptr<FILE> *>(impl);
        return !feof(ptr->get()) && !ferror(ptr->get());
    }
    return false;
}

namespace {
// prevents stdin/stdout/stderr from being closed at the end if they've not been moved-from
// this is especially annoying for stderr if you're using sanitizers
struct maybefclose {
    void operator()(FILE *p) const {
        if (close)
            fclose(p);
    }
    bool close = true;
};

struct console {
    console() {
        in.impl = new std::shared_ptr<FILE>(stdin, maybefclose());
        out.impl = new std::shared_ptr<FILE>(stdout, maybefclose());
        err.impl = new std::shared_ptr<FILE>(stderr, maybefclose());
    }
    ~console() {
        for (auto *instance : {&in, &out, &err}) {
            if (instance->impl) {
                auto f = *static_cast<std::shared_ptr<FILE> *>(instance->impl);
                std::get_deleter<maybefclose>(f)->close = false;
            }
        }
    }
};
} // namespace
file err;
file out;
file in;

console cons;

static void print_to_file(file &f, const var &v, const var &end = {}) {
    auto file = Underlying(f).get();
    auto var = v + "";
    if (end.type != var::null)
        var += end;
    fprintf(file, "%s", Str(var).data());
}

void file::print(std::initializer_list<var> vars, const var &end) {
    size_t i = 0;
    auto size = vars.size();
    var sep = " ";
    for (const auto &v : vars)
        print_to_file(*this, v, ++i < size ? sep : end);
}

void file::print(const var &fmt, std::initializer_list<var> vars, const var &end) {
    if (fmt.type != var::string)
        // you probably don't want to do this but what do i know
        return print_to_file(*this, fmt, end);

    auto it = vars.begin();
    auto varend = vars.end();

    size_t lastpos = 0;
    auto &str = Str(fmt);
    size_t pos;
    while ((pos = str.find("{}", lastpos)) != std::string::npos) {
        var v;
        v.type = var::string;
        new (v.u.buf) std::string(str.substr(lastpos, pos - lastpos));
        print_to_file(*this, v);
        lastpos = pos + 2;
        print_to_file(*this, it < varend ? *it++ : null);
    }

    var v;
    v.type = var::string;
    new (v.u.buf) std::string(str.substr(lastpos));

    print_to_file(*this, v, end);
}

void print(const var &v, const var &end) { print_to_file(out, v, end); }
void print(std::initializer_list<var> vars, const var &end) { out.print(vars, end); }
void print(const var &fmt, std::initializer_list<var> vars, const var &end) {
    out.print(fmt, vars, end);
}

var readline() { return in.readline(); }
dict readlines() { return in.readlines(); }
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
        new (ret.u.buf) std::string(line, l);
    }

    free(line);
    return ret;
}
dict file::readlines() {
    dict d;
    int i = 0;
    while (1) {
        auto tmp = readline();
        if (tmp)
            d[i++] = tmp;
        else
            break;
    }
    return d;
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
    new (ret.u.buf) std::string(tmp);

    return ret;
}
} // namespace io
