#pragma once

#include "var.hpp"
#include <initializer_list>

namespace Io {
struct File {
    void *impl = nullptr;

    File() = default;
    File(File &&);
    File(const File &);
    File &operator=(File &&);
    File &operator=(const File &);
    ~File();

    File(const var &);
};

extern File in;
extern File out;
extern File err;

void print(const var &);
void print(std::initializer_list<var>);
void print(File &, const var &);
void print(File &, std::initializer_list<var>);
void println(const var &);
void println(std::initializer_list<var>);
void println(File &, const var &);
void println(File &, std::initializer_list<var>);
} // namespace Io
