#include "var.hpp"
#include "dict.hpp"
#include <cmath>
#include <cstring>
#include <format>
#include <string>

#define Str(v) (*static_cast<std::string *>((v).u.str))
#define Int(v) (int((+(v)).u.num))
#define Long(v) (long((+(v)).u.num))
#define Double(v) ((+(v)).u.num)

static std::string ToString(const var &v) {
    switch (v.type) {
        case var::null:    return "null";
        case var::boolean: return v.u.b ? "true" : "false";
        case var::number:  return std::format("{}", v.u.num);
        case var::string:  return Str(v);
    }
    return "unreachable";
}
static var FromString(const std::string &s) {
    var ret;
    ret.type = var::string;
    ret.u.str = new std::string(s);
    return ret;
}

var &var::operator=(var &&other) {
    if (this != &other) {
        std::swap(this->type, other.type);
        std::swap(this->u.num, other.u.num);
    }
    return *this;
}
var &var::operator=(const var &other) {
    if (this != &other) {
        var tmp{other};
        std::swap(this->type, tmp.type);
        std::swap(this->u.num, tmp.u.num);
    }
    return *this;
}

var::var(bool val) {
    type = boolean;
    u.b = val;
}

#define num_constructor(t)                                                                         \
    var::var(t n) {                                                                                \
        type = number;                                                                             \
        u.num = n;                                                                                 \
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
    u.str = new std::string(s);
}

var::var(const var &other) {
    type = other.type;
    switch (type) {
        case null:    break;
        case boolean: u.b = other.u.b; break;
        case number:  u.num = other.u.num; break;
        case string:  u.str = new std::string(Str(other)); break;
    }
}
var::var(var &&other) {
    std::swap(type, other.type);
    std::swap(u.num, other.u.num);
}

var::~var() {
    if (type == string)
        delete &Str(*this);
}

var var::operator+() const {
    switch (type) {
        case null:    return 0;
        case boolean: return u.b ? 1 : 0;
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
        case boolean: return int(u.b) + int(other.u.b);
        case number:  return u.num + other.u.num;
        case string:  ;
    }

    return FromString(ToString(*this) + ToString(other));
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
    var var::operator op(const var & other) const { return Long(*this) op Long(other); }           \
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
            case boolean: return u.b op other.u.b;                                                 \
            case number:  return u.num op other.u.num;                                             \
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

var var::operator!() const { return !bool(*this); }
var var::operator&&(const var &other) const { return bool(*this) && bool(other); }
var var::operator||(const var &other) const { return bool(*this) || bool(other); }

var::operator bool() const {
    switch (type) {
        case null:    return false;
        case boolean: return u.b;
        case number:  return u.num != 0;
        case string:  return Str(*this) != "";
    }
    return false; // unreachable
}

var var::operator[](const var &pos, const var &count) const {
    auto self = ToString(*this);
    std::string result;

    auto off = Long(pos);
    auto len = Long(count);

    auto selfsize = long(self.size());

    if (len == -1) {
        if (off < selfsize)
            result += self[off];
    }
    else {
        for (long i = 0; i < len && off + i < selfsize; i++)
            result += self[off + i];
    }

    return FromString(result);
}

dict var::split(const var &s) const {
    auto self = ToString(*this);
    auto sep = ToString(s);

    dict d;

    size_t idx = 0;
    if (sep != "") {
        size_t prev_pos = 0, pos = 0;
        while ((pos = self.find(sep, pos)) != std::string::npos) {
            auto substr = self.substr(prev_pos, pos - prev_pos);
            d[idx++] = FromString(substr);
            prev_pos = ++pos;
        }

        auto substr = self.substr(prev_pos, pos - prev_pos);
        d[idx++] = FromString(substr);
    }
    else {
        for (auto c : self)
            d[idx++] = FromString({c});
    }

    return d;
}

var var::strip(const var &chars) const {
    auto self = ToString(*this);
    auto stripset = ToString(chars);

    // basically what strspn does, but not limited to c strings
    bool set[256]{};
    for (unsigned char c : stripset)
        set[c] = true;

    size_t prefix = 0;
    for (; prefix < self.size() && set[(unsigned char)self[prefix]]; prefix++)
        ;

    // strspn in reverse from the end
    size_t suffix = self.size();
    for (; suffix > 0 && set[(unsigned char)self[suffix - 1]]; suffix--)
        ;

    return FromString(self.substr(prefix, suffix - prefix));
}
