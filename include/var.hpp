#pragma once

struct dict;
struct var {
    enum {
        null,
        boolean,
        number,
        string,
    } type = null;

    union {
        bool b;
        double num;
        char buf[32];
    } u;

    var() = default;
    var(var &&);
    var(const var &);
    var &operator=(var &&);
    var &operator=(const var &);

    var(bool);
    var(char);
    var(short);
    var(int);
    var(long);
    var(long long);
    var(unsigned char);
    var(unsigned short);
    var(unsigned int);
    var(unsigned long);
    var(unsigned long long);
    var(float);
    var(double);
    var(long double);
    var(const char *);
    var(const char *, unsigned long);
    ~var();

    var &operator++();
    var &operator--();
    var operator++(int);
    var operator--(int);

    var &operator+=(const var &);
    var &operator-=(const var &);
    var &operator*=(const var &);
    var &operator/=(const var &);
    var &operator%=(const var &);

    var &operator&=(const var &);
    var &operator|=(const var &);
    var &operator^=(const var &);

    var &operator<<=(const var &);
    var &operator>>=(const var &);

    explicit operator bool() const;

    var operator[](const var &pos, const var &count = -1) const;

    var len() const;
    dict split(const var &sep = " ") const;
    var strip(const var &chars = " \n") const;
};

static const var null{};
var operator+(const var &v);
var operator-(const var &v);
var operator+(const var &lhs, const var &rhs);
var operator-(const var &lhs, const var &rhs);
var operator*(const var &lhs, const var &rhs);
var operator/(const var &lhs, const var &rhs);
var operator%(const var &lhs, const var &rhs);
var operator~(const var &v);
var operator&(const var &lhs, const var &rhs);
var operator|(const var &lhs, const var &rhs);
var operator^(const var &lhs, const var &rhs);
var operator<<(const var &lhs, const var &rhs);
var operator>>(const var &lhs, const var &rhs);
var operator<(const var &lhs, const var &rhs);
var operator>(const var &lhs, const var &rhs);
var operator<=(const var &lhs, const var &rhs);
var operator>=(const var &lhs, const var &rhs);
var operator==(const var &lhs, const var &rhs);
var operator!=(const var &lhs, const var &rhs);
var operator!(const var &v);
var operator&&(const var &lhs, const var &rhs);
var operator||(const var &lhs, const var &rhs);

var operator""_v(const char *v, unsigned long);
var operator""_v(unsigned long long int v);
var operator""_v(long double v);
