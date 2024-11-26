#include "var.hpp"
#include "dict.hpp"
#include "internal.hpp"
#include <cmath>
#include <cstring>
#include <string>

// this is a bit of a hack because var.hpp can't include std::string
// if the assert fails maybe manually change the buffer size idk
static_assert(sizeof(std::string) <= sizeof(var::u));
static_assert(alignof(std::string) <= alignof(decltype(var::u)));

// std::string in libstdc++ has a pointer to an internal buffer
var &var::operator=(var &&other) {
    if (this != &other) {
        // todo: abstract this away
        if (type == string)
            std::destroy_at(&Str(*this));
        type = std::move(other.type);

        switch (type) {
            case null:    break;
            case boolean: u.b = other.u.b; break;
            case number:  u.num = other.u.num; break;
            case string:  new (u.buf) std::string(std::move(Str(other))); break;
        }
    }
    return *this;
}
var &var::operator=(const var &other) {
    if (this != &other) {
        var tmp{other};

        if (type == string)
            std::destroy_at(&(Str(*this)));
        type = std::move(tmp.type);

        switch (type) {
            case null:    break;
            case boolean: u.b = other.u.b; break;
            case number:  u.num = other.u.num; break;
            case string:  new (u.buf) std::string(std::move(Str(tmp))); break;
        }
    }
    return *this;
}

var::var(bool val) {
    type = boolean;
    u.b  = val;
}

#define num_constructor(t)                                                                         \
    var::var(t n) {                                                                                \
        type  = number;                                                                            \
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
    new (u.buf) std::string(s);
}
var::var(const char *s, unsigned long len) {
    type = string;
    new (u.buf) std::string(s, len);
}

var::var(const var &other) {
    type = other.type;
    switch (type) {
        case null:    break;
        case boolean: u.b = other.u.b; break;
        case number:  u.num = other.u.num; break;
        case string:  new (u.buf) std::string(Str(other)); break;
    }
}
var::var(var &&other) {
    type = std::move(other.type);
    switch (type) {
        case null:    break;
        case boolean: u.b = other.u.b; break;
        case number:  u.num = other.u.num; break;
        case string:  new (u.buf) std::string(std::move(Str(other))); break;
    }
}

var::~var() {
    if (type == string)
        std::destroy_at(&(Str(*this)));
}

var operator+(const var &v) {
    switch (v.type) {
        case var::null:    return 0;
        case var::boolean: return v.u.b ? 1 : 0;
        case var::number:  return v;
        case var::string:  return strtod(Str(v).data(), nullptr);
    }
    return 0; // unreachable
}
var operator-(const var &v) { return -Double(v); }

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

var operator+(const var &lhs, const var &rhs) {
    switch (common_type(lhs, rhs)) {
        case var::null:    return {};
        case var::boolean: return int(lhs.u.b) + int(rhs.u.b);
        case var::number:  return Double(lhs) + Double(rhs);
        case var::string:  ;
    }

    return FromString(ToString(lhs) + ToString(rhs));
}
var operator-(const var &lhs, const var &rhs) { return Double(lhs) - Double(rhs); }
var operator*(const var &lhs, const var &rhs) {
    /*
     * null * x = null
     * x * null = null
     *
     * true * x = x
     * x * true = x
     *
     * false * x = null
     * x * false = null
     *
     * str * num = num copies of str
     * num * str = num copies of str
     *
     * num * num = normal mul
     * str * str = +str * +str
     */

    const var *lhsp, *rhsp;
    if (lhs.type == std::min(lhs.type, rhs.type)) {
        lhsp = &lhs;
        rhsp = &rhs;
    }
    else {
        lhsp = &rhs;
        rhsp = &lhs;
    }

    switch (lhsp->type) {
        case var::null:    return var{};
        case var::boolean: return lhsp->u.b ? *rhsp : var{};
        case var::string:  return Double(*lhsp) * Double(*rhsp);
        case var::number:
            if (rhsp->type == var::number)
                return lhsp->u.num * rhsp->u.num;
            std::string s;
            for (long l = 0; l < lhsp->u.num; l++)
                s += Str(*rhsp);
            return FromString(s);
    }
}

var operator/(const var &lhs, const var &rhs) {
    auto divisor = Double(rhs);
    if (divisor == 0)
        return {};
    return Double(lhs) / divisor;
}
var operator%(const var &lhs, const var &rhs) {
    auto divisor = Double(rhs);
    if (divisor == 0)
        return {};
    return std::fmod(Double(lhs), divisor);
}

var &var::operator+=(const var &other) { return *this = *this + other; }
var &var::operator-=(const var &other) { return *this = *this - other; }
var &var::operator*=(const var &other) { return *this = *this * other; }
var &var::operator/=(const var &other) { return *this = *this / other; }
var &var::operator%=(const var &other) { return *this = *this % other; }

#define bit(op)                                                                                    \
    var &var::operator op##=(const var & other) { return *this = *this op Long(other); }           \
    var operator op(const var &lhs, const var &rhs) { return Long(lhs) op Long(rhs); }

var operator~(const var &v) { return ~Int(v); }
bit(&);
bit(|);
bit(^);
bit(<<);
bit(>>);

#define cmp(op)                                                                                    \
    var operator op(const var &lhs, const var &rhs) {                                              \
        switch (common_type(lhs, rhs)) {                                                           \
            case var::null:    return 0 op 0;                                                      \
            case var::boolean: return lhs.u.b op rhs.u.b;                                          \
            case var::number:  return lhs.u.num op rhs.u.num;                                      \
            case var::string:  return ToString(lhs) op ToString(rhs);                               \
        }                                                                                          \
        return false; /* unreachable */                                                            \
    }

cmp(<);
cmp(>);
cmp(<=);
cmp(>=);
cmp(==);
cmp(!=);

var operator!(const var &v) { return !bool(v); }
var operator&&(const var &lhs, const var &rhs) { return bool(lhs) && bool(rhs); }
var operator||(const var &lhs, const var &rhs) { return bool(lhs) || bool(rhs); }

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

    auto selfsize = long(self.size());

    auto off = Long(pos);
    auto len = count.type == null ? selfsize : Long(count);

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
    auto sep  = ToString(s);

    dict d;

    size_t idx = 0;
    if (sep != "") {
        size_t prev_pos = 0, pos = 0;
        while ((pos = self.find(sep, pos)) != std::string::npos) {
            auto substr = self.substr(prev_pos, pos - prev_pos);
            d[idx++]    = FromString(substr);
            prev_pos    = ++pos;
        }

        auto substr = self.substr(prev_pos, pos - prev_pos);
        d[idx++]    = FromString(substr);
    }
    else {
        for (auto c : self)
            d[idx++] = FromString({c});
    }

    return d;
}

var var::strip(const var &chars) const {
    auto self     = ToString(*this);
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

var var::len() const {
    if (type == string)
        return Str(*this).size();
    auto v = *this + "";
    return Str(v).size();
}

var operator""_v(const char *v, unsigned long l) { return {v, l}; }
var operator""_v(unsigned long long int v) { return v; }
var operator""_v(long double v) { return v; }
