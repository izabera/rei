#pragma once

#include "var.hpp"
#include <initializer_list>

namespace io {
struct file {
    void *impl = nullptr;

    file() = default;
    file(file &&);
    file(const file &);
    file &operator=(file &&);
    file &operator=(const file &);
    ~file();

    file(const var &);
};

extern file in;
extern file out;
extern file err;

void print(const var &);
void print(std::initializer_list<var>);
void print(file &, const var &);
void print(file &, std::initializer_list<var>);
void println(const var &);
void println(std::initializer_list<var>);
void println(file &, const var &);
void println(file &, std::initializer_list<var>);
} // namespace Io
