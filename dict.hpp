#pragma once
#include "var.hpp"

struct dict {
    void *impl;

    dict();
    dict(dict&&);
    dict(const dict&);
    dict& operator=(dict&&);
    dict& operator=(const dict&);

    var& operator[](const var&);
};

#define in ->*
bool operator->*(const var&, const dict&);
