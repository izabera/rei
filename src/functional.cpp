#include "functional.hpp"

namespace fn {
var toint(const var &v) { return v | 0; }
var tonum(const var &v) { return +v; }
var tostr(const var &v) { return v + ""; }

var typeof(const var &v) {
    switch (v.type) {
        case var::null:    return "null";
        case var::boolean: return "boolean";
        case var::number:  return "number";
        case var::string:  return "string";
    }
    return "unreachable";
}

var abs(const var& v) {
    return v < 0 ? -v : v;
}
} // namespace fn
