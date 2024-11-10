#include "dict.hpp"
#include "var.hpp"
#include "functional.hpp"
#include <map>

// the default comparison in libstdc++'s std map doesn't call explicit operator bool :(
using cmp = decltype([](const var &lhs, const var &rhs) { return bool{lhs < rhs}; });

using maptype = std::map<var, var, cmp>;

#define Map(v) (*static_cast<maptype *>((v).impl))

var &dict::operator[](const var &v) { return Map(*this)[v]; }

bool dict::contains(const var &v) const { return Map(*this).contains(v); }

dict::dict() { impl = new maptype; }
dict::dict(const dict &other) { impl = new maptype(Map(other)); }
dict::dict(dict &&other) { std::swap(this->impl, other.impl); }
dict &dict::operator=(const dict &other) {
    if (this != &other) {
        dict tmp{other};
        std::swap(this->impl, tmp.impl);
    }
    return *this;
}
dict &dict::operator=(dict &&other) {
    if (this != &other)
        std::swap(this->impl, other.impl);
    return *this;
}

dict::iter dict::begin() const { return dict::iter{0, this}; }
dict::iter dict::end() const { return dict::iter{Map(*this).size(), this}; }
dict::iter &dict::iter::operator++() {
    pos++;
    return *this;
}
dict::kv dict::iter::operator*() const {
    auto it = std::next(Map(*map).begin(), pos);
    return {it->first, it->second};
}
var dict::size() const { return Map(*this).size(); }

dict dict::map(fn::unary f) const {
    dict ret;
    for (auto [k, v] : *this)
        ret[k] = f(v);
    return ret;
}

dict dict::filter(fn::unary f) const {
    dict ret;
    for (auto [k, v] : *this)
        if (f(v))
            ret[k] = v;
    return ret;
}

var dict::reduce(fn::binary f) const {
    auto map = Map(*this);
    auto it = map.begin();
    auto end = map.end();

    if (map.size() == 0)
        return {};

    auto ret = it->second;
    while (++it != end)
        ret = f(ret, it->second);

    return ret;
}

var dict::join(const var &sep) const {
    var ret;
    int i = 0;
    for (auto [k, v] : Map(*this)) {
        if (i++ && sep.type != var::null)
            ret += sep;
        ret += v;
    }
    return ret;
}
