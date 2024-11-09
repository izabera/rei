#pragma once

struct var;
struct dict {
    void *impl;

    dict();
    dict(dict &&);
    dict(const dict &);
    dict &operator=(dict &&);
    dict &operator=(const dict &);

    var &operator[](const var &);

    bool contains(const var &) const;
};
