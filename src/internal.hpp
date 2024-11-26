#pragma once
#include "var.hpp"
#include <string>
#include <format>

template <typename t1, typename t2>
using same_const_t = std::conditional_t<std::is_const_v<std::remove_reference_t<t1>>, const t2, t2>;

#define Str(v) (*reinterpret_cast<same_const_t<decltype(v), std::string> *>(&(v).u.buf))
#define Int(v) (int((+(v)).u.num))
#define Long(v) (long((+(v)).u.num))
#define Double(v) ((+(v)).u.num)

static inline var FromString(const std::string &s) {
    var ret;
    ret.type = var::string;
    new (ret.u.buf) std::string(s);
    return ret;
}

static inline std::string ToString(const var &v) {
    switch (v.type) {
        case var::null:    return "null";
        case var::boolean: return v.u.b ? "true" : "false";
        case var::string:  return Str(v);
        case var::number:
            // this avoids printing 300000 as 3e+05 (or 300000.000000 if one used {:f})
            // it's dumb but there is no good way in fmt/std::format to do automatic precision
            // a custom formatter would really have to do the same thing
            if (v.u.num == long(v.u.num))
                return std::format("{}", long(v.u.num));
            return std::format("{}", v.u.num);
    }
    return "unreachable";
}
