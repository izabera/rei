#include "var.hpp"
#include <cmath>
#include <format>
#include <string>

#define Str(v) (*static_cast<std::string *>((v).str))
#define Int(v) (int((+(v)).num))
#define Double(v) ((+(v)).num)

static auto ToString = [](const var &v) {
    if (v.type == var::number)
        return std::format("{}", v.num);
    return Str(v);
};

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
var::var(double n) {
    type = number;
    num = n;
}
var::var(const char *s) {
    type = string;
    str = new std::string(s);
}
var::var(const var &other) {
    type = other.type;
    if (type == number)
        num = other.num;
    else
        str = new std::string(Str(other));
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
    if (type == number)
        return *this;
    return strtod(Str(*this).data(), nullptr);
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

var var::operator+(const var &other) const {
    if (type == number && other.type == number)
        return num + other.num;

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
        if (type == number && other.type == number)                                                \
            return num op other.num;                                                               \
        return ToString(*this) op ToString(other);                                                 \
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

var::operator bool() const { return type == number ? num != 0 : Str(*this) == ""; }
