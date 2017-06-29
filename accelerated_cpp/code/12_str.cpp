#include <iostream>

#include "11_ADT.cpp"

class Str
{
    friend std::istream &operator>>(std::istream &, Str &);

  public:
    // always return a reference for assignment operators
    Str &operator+=(const Str &s)
    {
        std::copy(s.data.begin(), s.data.end(), std::back_inserter(data));
        return *this;
    }

    typedef Vec<char>::size_type size_type;

    // default constructor
    Str() {}
    // create Str with n copies of c
    Str(size_type n, char c) : data(n, c) {}

    // create a Str from a null-terminated array of char
    Str(const char *cp)
    {
        std::copy(cp, cp + std::strlen(cp), std::back_inserter(data));
    }

    // create Str from range denoted by iterators b and e
    template <class In>
    Str(In b, In e)
    {
        std::copy(b, e, std::back_inserter(data));
    }

    // operators
    // index
    char &operator[](size_type i) { return data[i]; }
    const char &operator[](size_type i) const { return data[i]; }

    // member function
    size_type size() const { return data.size(); }

  private:
    Vec<char> data;
};

// nonmember functions
std::istream &operator>>(std::istream &, Str &);
std::ostream &operator<<(std::ostream &, const Str &);
Str operator+(const Str &, const Str &);

std::istream &operator>>(std::istream &is, Str &s)
{
    // obliterate existing value
    s.data.clear();

    // read and discard leading whitespace
    char c;
    while (is.get(c) && isspace(c))
        ;

    // read in until next whitespace
    if (is)
    {
        s.data.push_back(c);
        while (is.get(c) && !isspace(c))
            ;

        if (is)
            is.unget(); // put the last read space into stream...
    }

    return is;
}

std::ostream &operator<<(std::ostream &os, const Str &s)
{
    for (Str::size_type i = 0; i != s.size(); ++i)
        os << s[i]; // Vec::operator[] is invoked here
    return os;
}

Str operator+(const Str &s, const Str &t)
{
    Str r = s; // initialized local variable with Str copy constructor
    r += t;    // concatenation
    return r;  // implicity copy constructor to return r
}