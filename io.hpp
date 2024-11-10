#pragma once

#include "var.hpp"
#include <initializer_list>

namespace io {

enum mode { r, w, rw };

struct file {
    void *impl = nullptr;

    file() = default;
    file(file &&);
    file(const file &);
    file &operator=(file &&);
    file &operator=(const file &);
    ~file();

    file(const var &name, mode = r);

    void print(const var &);
    void print(std::initializer_list<var>);
    void println(const var &);
    void println(std::initializer_list<var>);

    var read(const var &len = {});
    var readline();
};

extern file in;
extern file out;
extern file err;

void print(const var &);
void print(std::initializer_list<var>);
void println(const var &);
void println(std::initializer_list<var>);
} // namespace io
