#include "dict.hpp"
#include <map>

// the default comparison in libstdc++'s std map doesn't call explicit operator bool :(
using cmp = decltype([](const var &lhs, const var &rhs) { return bool{lhs < rhs}; });

using maptype = std::map<var, var, cmp>;

#define Map(v) (*static_cast<maptype *>((v).impl))

var &dict::operator[](const var &v) { return Map(*this)[v]; }

bool operator->*(const var &v, const dict &d) { return Map(d).contains(v); }

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
