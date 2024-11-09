#pragma once

struct var {
    enum {
        boolean,
        number,
        string,
    } type = number;

    union {
        bool b;
        double num = 0;
        void *str;
    };

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
    ~var();

    var operator+() const;
    var operator-() const;

    var &operator++();
    var &operator--();
    var operator++(int);
    var operator--(int);

    var operator+(const var &) const;
    var operator-(const var &) const;
    var operator*(const var &) const;
    var operator/(const var &) const;
    var operator%(const var &) const;
    var &operator+=(const var &);
    var &operator-=(const var &);
    var &operator*=(const var &);
    var &operator/=(const var &);
    var &operator%=(const var &);

    var operator~() const;
    var operator&(const var &) const;
    var operator|(const var &) const;
    var operator^(const var &) const;
    var &operator&=(const var &);
    var &operator|=(const var &);
    var &operator^=(const var &);

    var operator<<(const var &) const;
    var operator>>(const var &) const;
    var &operator<<=(const var &);
    var &operator>>=(const var &);

    var operator<(const var &) const;
    var operator>(const var &) const;
    var operator<=(const var &) const;
    var operator>=(const var &) const;
    var operator==(const var &) const;
    var operator!=(const var &) const;

    var operator!() const;
    var operator&&(const var &) const;
    var operator||(const var &) const;

    explicit operator bool() const;
};
