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

    void print(std::initializer_list<var>, const var& end = "\n");
    void print(const var &fmt, std::initializer_list<var> = {}, const var &end = "\n");

    var read(const var &len = {});
    var readline();
};

extern file in;
extern file out;
extern file err;

void print(std::initializer_list<var>, const var& end = "\n");
void print(const var &fmt, std::initializer_list<var> = {}, const var &end = "\n");

var read(const var &len = {});
var readline();
} // namespace io
