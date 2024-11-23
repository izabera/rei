#pragma once
#include "var.hpp"

namespace fn {
using unary = var (*)(const var &);
using binary = var (*)(const var &, const var &);

var toint(const var &v);
var tonum(const var &v);
var tostr(const var &v);

var typeof(const var &v);
} // namespace fn
