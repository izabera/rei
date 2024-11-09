#include "var.hpp"
#include <cmath>
#include <format>
#include <string>

#define Str(v) (*static_cast<std::string *>((v).str))
#define Int(v) (int((+(v)).num))
#define Double(v) ((+(v)).num)

static std::string ToString(const var &v) {
    switch (v.type) {
        case var::null:    return "null";
        case var::boolean: return v.b ? "true" : "false";
        case var::number:  return std::format("{}", v.num);
        case var::string:  return Str(v);
    }
    return "unreachable";
}

var &var::operator=(var &&other) {
    if (this != &other) {
        std::swap(this->type, other.type);
        std::swap(this->num, other.num);
    }
    return *this;
}
var &var::operator=(const var &other) {
    if (this != &other) {
        var tmp{other};
        std::swap(this->type, tmp.type);
        std::swap(this->num, tmp.num);
    }
    return *this;
}

var::var(bool val) {
    type = boolean;
    b = val;
}

#define num_constructor(t)                                                                         \
    var::var(t n) {                                                                                \
        type = number;                                                                             \
        num = n;                                                                                   \
    }

num_constructor(char);
num_constructor(short);
num_constructor(int);
num_constructor(long);
num_constructor(long long);
num_constructor(unsigned char);
num_constructor(unsigned short);
num_constructor(unsigned int);
num_constructor(unsigned long);
num_constructor(unsigned long long);
num_constructor(float);
num_constructor(double);
num_constructor(long double);

var::var(const char *s) {
    type = string;
    str = new std::string(s);
}

var::var(const var &other) {
    type = other.type;
    switch (type) {
        case null:    break;
        case boolean: b = other.b; break;
        case number:  num = other.num; break;
        case string:  str = new std::string(Str(other)); break;
    }
}
var::var(var &&other) {
    std::swap(type, other.type);
    std::swap(num, other.num);
}

var::~var() {
    if (type == string)
        delete &Str(*this);
}

var var::operator+() const {
    switch (type) {
        case null:    return 0;
        case boolean: return b ? 1 : 0;
        case number:  return *this;
        case string:  return strtod(Str(*this).data(), nullptr);
    }
    return 0; // unreachable
}
var var::operator-() const { return -Double(*this); }

var &var::operator++() { return *this = Double(*this) + 1; }
var &var::operator--() { return *this = Double(*this) - 1; }
var var::operator++(int) {
    var tmp = +*this;
    ++(*this);
    return tmp;
}
var var::operator--(int) {
    var tmp = +*this;
    --(*this);
    return tmp;
}

static auto common_type(const var &lhs, const var &rhs) { return std::max(lhs.type, rhs.type); }

var var::operator+(const var &other) const {
    switch (common_type(*this, other)) {
        case null:    return 0;
        case boolean: return int(b) + int(other.b);
        case number:  return num + other.num;
        case string:  ;
    }

    var ret;
    ret.type = string;
    ret.str = new std::string(ToString(*this) + ToString(other));
    return ret;
}
var var::operator-(const var &other) const { return Double(*this) - Double(other); }
var var::operator*(const var &other) const { return Double(*this) * Double(other); }
var var::operator/(const var &other) const { return Double(*this) / Double(other); }
var var::operator%(const var &other) const { return std::remainder(Double(*this), Double(other)); }

var &var::operator+=(const var &other) { return *this = *this + other; }
var &var::operator-=(const var &other) { return *this = *this - other; }
var &var::operator*=(const var &other) { return *this = *this * other; }
var &var::operator/=(const var &other) { return *this = *this / other; }
var &var::operator%=(const var &other) { return *this = *this % other; }

#define bit(op)                                                                                    \
    var var::operator op(const var & other) const { return Int(*this) op Int(other); }             \
    var &var::operator op##=(const var & other) { return *this = *this op other; }

var var::operator~() const { return ~Int(*this); }
bit(&);
bit(|);
bit(^);
bit(<<);
bit(>>);

#define cmp(op)                                                                                    \
    var var::operator op(const var & other) const {                                                \
        switch (common_type(*this, other)) {                                                       \
            case null:    return true;                                                             \
            case boolean: return b op other.b;                                                     \
            case number:  return num op other.num;                                                 \
            case string:  return ToString(*this) op ToString(other);                                \
        }                                                                                          \
        return false; /* unreachable */                                                            \
    }

cmp(<);
cmp(>);
cmp(<=);
cmp(>=);
cmp(==);
cmp(!=);

var var::operator!() const { return *this ? 1 : 0; }
var var::operator&&(const var &other) const { return *this ? *this : other; }
var var::operator||(const var &other) const { return *this ? other : *this; }

var::operator bool() const {
    switch (type) {
        case null:    return false;
        case boolean: return b;
        case number:  return num != 0;
        case string:  return Str(*this) != "";
    }
    return false; // unreachable
}
