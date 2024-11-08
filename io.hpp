#pragma once
#include "var.hpp"

struct io {
    void *impl = nullptr;

    io() = default;
    io(io &&) = default;
    io(const io &) = default;
    io &operator=(io &&);
    io &operator=(const io &);
    ~io();

    io(const var &);

    io &operator>>(var &);
    io &operator<<(const var &);

    static io in;
    static io out;
    static io err;
};
