#include "dict.hpp"
#include "functional.hpp"
#include "var.hpp"
#include <map>
#include <vector>

// the default comparison in libstdc++'s std map doesn't call explicit operator bool :(
using cmp = decltype([](const var &lhs, const var &rhs) { return bool{lhs < rhs}; });

using maptype = std::map<var, var, cmp>;
using vectype = std::vector<var>;

#define Map(v) (*static_cast<maptype *>((v).impl))
#define Vec(v) (*static_cast<vectype *>((v).impl))

var &dict::operator[](const var &v) {
    if (type != contiguous)
        return Map(*this)[v];

    auto &vec = Vec(*this);

    if (v.type == var::number) {
        if (v.u.num == long(v.u.num) && v.u.num >= 0) {
            // access one past the end is ok, just create the var
            // maybe this should be allowed for n past the end too
            // todo: find a good compromise
            if (v.u.num == vec.size())
                vec.push_back({});
            if (v.u.num < vec.size())
                return vec[v.u.num];
        }
    }

    // could not find this key in the existing vector, so move everything to a map
    auto map = new maptype;
    for (long i = 0; i < long(vec.size()); i++)
        (*map)[i] = std::move(vec[i]);

    delete &Vec(*this);
    impl = map;
    type = sparse;
    return Map(*this)[v];
}

bool dict::contains(const var &v) const {
    if (v.type == var::number)
        if (v.u.num == long(v.u.num) && v.u.num >= 0 && v.u.num < Vec(*this).size())
            return true;
    return Map(*this).contains(v);
}

dict::dict() { impl = new vectype; }
dict::~dict() {
    if (impl != nullptr) {
        if (type == contiguous)
            delete &Vec(*this);
        else
            delete &Map(*this);
    }
}
dict::dict(const dict &other) {
    type = other.type;
    if (type == contiguous)
        impl = new vectype(Vec(other));
    else
        impl = new maptype(Map(other));
}
dict::dict(dict &&other) {
    impl = other.impl;
    type = other.type;
    other.impl = nullptr;
}
dict &dict::operator=(const dict &other) {
    if (this != &other) {
        dict tmp{other};
        std::swap(impl, tmp.impl);
        std::swap(type, tmp.type);
    }
    return *this;
}
dict &dict::operator=(dict &&other) {
    if (this != &other) {
        std::swap(impl, other.impl);
        std::swap(type, other.type);
    }
    return *this;
}

dict::iter dict::begin() const { return dict::iter{0, this}; }
dict::iter dict::end() const { return dict::iter{static_cast<unsigned long>(size().u.num), this}; }
dict::iter &dict::iter::operator++() {
    pos++;
    return *this;
}
dict::kv dict::iter::operator*() const {
    if (map->type == contiguous)
        return {pos, Vec(*map)[pos]};

    auto it = std::next(Map(*map).begin(), pos);
    return {it->first, it->second};
}
var dict::size() const { return type == contiguous ? Vec(*this).size() : Map(*this).size(); }

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
