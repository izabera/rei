#pragma once

#include "var.hpp"

struct dict {
    void *impl;
    enum {
        contiguous,
        sparse,
    } type = contiguous;

    dict();
    dict(dict &&);
    dict(const dict &);
    dict &operator=(dict &&);
    dict &operator=(const dict &);
    ~dict();

    var &operator[](const var &);
    bool contains(const var &) const;
    var size() const;
    void resize(const var &size);
    void remove(const var &key);

    struct kv {
        var key;
        var value;
    };
    struct iter {
        unsigned long pos = 0;
        const dict *map;
        iter &operator++();
        kv operator*() const;
        bool operator==(const iter &) const = default;
        bool operator!=(const iter &) const = default;
    };
    iter begin() const;
    iter end() const;

    dict map(var (*unary)(const var &)) const;
    var reduce(var (*binary)(const var &, const var &)) const;
    dict filter(var (*unary)(const var &)) const;
    var join(const var &sep = {}) const;
    void sort();
};
