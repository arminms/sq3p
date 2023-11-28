//
// Copyright (c) 2023 Armin Sobhani
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#ifndef _SQ3P_SQ_HPP_
#define _SQ3P_SQ_HPP_

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <any>
#include <initializer_list>
#include <utility>
#include <stdexcept>
#include <typeindex>
#include <iomanip>
namespace sq3p {

template<class T, class F>
inline
std::pair<const std::type_index, std::function<void(std::ostream&, const std::any&)>>
    make_td_print_visitor(const F& f)
{   return
    {   std::type_index(typeid(T)),
        [g = f](std::ostream& os, const std::any& a)
        {   if constexpr (std::is_void_v<T>) g(os);
            else g(os, std::any_cast<const T&>(a));
        }
    };
}

static std::unordered_map<std::type_index, std::function<void(std::ostream&, const std::any&)>>
    td_print_visitor
{   make_td_print_visitor<void>
    (   [] (std::ostream& os)
        { os << std::quoted("void", '|') << "{}"; }
    )
,   make_td_print_visitor<bool>
    (   [](std::ostream& os, bool x)
        { os << std::quoted("bool", '|') << x; }
    )
,   make_td_print_visitor<int>
    (   [](std::ostream& os, int x)
        { os << std::quoted("int", '|') << x; }
    )
,   make_td_print_visitor<unsigned>
    (   [](std::ostream& os, unsigned x)
        { os << std::quoted("unsigned", '|') << x; }
    )
,   make_td_print_visitor<float>
    (   [](std::ostream& os, float x)
        { os << std::quoted("float", '|') << x; }
    )
,   make_td_print_visitor<double>
    (   [](std::ostream& os, double x)
        { os << std::quoted("double", '|') << x; }
    )
,   make_td_print_visitor<std::string>
    (   [] (std::ostream& os, std::string s)
        { os << std::quoted("string", '|') << std::quoted(s); }
    )
,   make_td_print_visitor<std::vector<int>>
    (   [] (std::ostream& os, std::vector<int> v)
        {   os << std::quoted("std::vector<int>", '|') << '{';
            for (auto i : v)
                os << i << ',';
            os << '}';
        }
    )
    // ... add more handlers here ...
};

static std::unordered_map<std::string, std::function<void(std::istream&, std::any&)>>
    td_scan_visitor
{   {   "void"
    ,   [] (std::istream& is, std::any& a)
        { is.ignore(2) ; a = {}; }
    }
    ,
    {   "bool"
    ,   [] (std::istream& is, std::any& a)
        { bool x; is >> x; a = x; }
    }
    ,
    {   "int"
    ,   [] (std::istream& is, std::any& a)
        { int x; is >> x; a = x; }
    }
    ,
    {   "unsigned"
    ,   [] (std::istream& is, std::any& a)
        { unsigned x; is >> x; a = x; }
    }
    ,
    {   "float"
    ,   [] (std::istream& is, std::any& a)
        { float x; is >> x; a = x; }
    }
    ,
    {   "double"
    ,   [] (std::istream& is, std::any& a)
        { double x; is >> x; a = x; }
    }
    ,
    {   "string"
    ,   [] (std::istream& is, std::any& a)
        { std::string s; is >> std::quoted(s); a = s; }
    }
    ,
    {   "std::vector<int>"
    ,   [] (std::istream& is, std::any& a)
        {   std::vector<int> v;
            int i;
            is.ignore();
            while (is.peek() != '}')
            {   is >> i;
                is.ignore();
                v.push_back(i);
            }
            is.ignore();
            a = v;
        }
    }
    ,
    {   "UNREGISTERED TYPE"
    ,   [] (std::istream& is, std::any& a)
        { is.ignore(2) ; a = {}; }
    }
    // ... add more handlers here ...
};

template <typename Container>
class sq_gen
{   Container                                 _sq;  // sequence
    std::unordered_map<std::string, std::any> _td;  // tagged data

public:
    using value_type = typename Container::value_type;
    using size_type = typename Container::size_type;
    using difference_type = typename Container::difference_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;
    using iterator = typename Container::iterator;
    using const_iterator = typename Container::const_iterator;
    using reverse_iterator = typename Container::reverse_iterator;
    using const_reverse_iterator = typename Container::const_reverse_iterator;

// -- constructors -------------------------------------------------------------

    sq_gen() noexcept
    :   _sq()
    ,   _td()
    {}
    explicit sq_gen(std::string sq)
    :   _sq(std::begin(sq), std::end(sq))
    ,   _td()
    {}
    sq_gen(size_type count, const_reference value = value_type(65))
    :   _sq(count, value)
    ,   _td()
    {}
    template<typename InputIt>
    sq_gen(InputIt first, InputIt last)
    :   _sq(first, last)
    ,   _td()
    {}
    sq_gen(const sq_gen& other)
    :   _sq(other._sq)
    ,   _td(other._td)
    {}
    sq_gen(sq_gen&& other)
    :   _sq(std::move(other._sq))
    ,   _td(std::move(other._td))
    {}
    sq_gen(std::initializer_list<value_type> init)
    :   _sq(init)
    ,   _td()
    {}

// -- copy assignment operators ------------------------------------------------

    sq_gen& operator= (const sq_gen& other)
    {   _sq = other._sq;
        _td = other._td;
        return *this;
    }
    sq_gen& operator= (sq_gen&& other)
    {   _sq = std::move(other._sq);
        _td = std::move(other._td);
        return *this;
    }
    sq_gen& operator= (std::initializer_list<value_type> init)
    {   _sq = init;
        return *this;
    }

// -- capacity -----------------------------------------------------------------

    /// Returns true if the @a sq is empty. (Thus begin() would equal end().)
    bool empty() const noexcept
    {   return (_sq.empty() && _td.empty());   }
    size_type size() const noexcept
    {   return _sq.size();   }

// -- managing tagged data -----------------------------------------------------

    bool has(std::string tag) const
    {   return _td.find(tag) == _td.end() ? false : true;  }
    std::any& operator[] (const std::string& tag)
    {   return _td[tag];   }
    std::any& operator[] (std::string&& tag)
    {   return _td[std::forward<std::string>(tag)];   }

// -- subscript operator -------------------------------------------------------

    reference operator[] (size_type pos)
    {   return _sq[pos];   }
    const_reference operator[] (size_type pos) const
    {   return _sq[pos];   }

// -- comparison operators -----------------------------------------------------

    template<typename Container1, typename Container2>
    friend
    bool operator== (const sq_gen<Container1>& lhs, const sq_gen<Container2>& rhs)
    {   return lhs._sq == rhs._sq;   }
    template<typename Container1, typename Container2>
    friend
    bool operator!= (const sq_gen<Container1>& lhs, const sq_gen<Container2>& rhs)
    {   return lhs._sq != rhs._sq;   }

// -- subseq operator ----------------------------------------------------------

    sq_gen operator() (size_type pos, size_type count = std::string::npos) const
    {   if (pos > _sq.size())
            throw std::out_of_range("sq3p::sq: pos > this->size()");
        return sq_gen
        (   _sq.begin() + pos
        ,   (count > _sq.size() - pos) ? _sq.end() : _sq.begin() + pos + count
        );
    }

// -- file i/o -----------------------------------------------------------------

    template<template <class> class Format>
    bool load(std::string filename, std::string id, Format<Container> f)
    {   return f(*this, filename, id);   }

    void print(std::ostream& os) const
    {   os << std::boolalpha << _sq.size();
        os.write(_sq.data(), _sq.size());
        for (const auto& [tag, data] : _td)
        {   os << std::quoted(tag, '#');
            if
            (   const auto it = td_print_visitor.find(std::type_index(data.type()))
            ;    it != td_print_visitor.cend()
            )
                it->second(os, data);
            else
                os << std::quoted("UNREGISTERED TYPE", '|')
                //    << std::quoted(data.type().name())
                   << "{}";
        }
    }
    void scan(std::istream& is)
    {   size_type n;
        is >> std::boolalpha >> n;
        _sq.resize(n);
        is.read(_sq.data(), n);
        while (is.peek() == '#')
        {   std::string tag, type;
            std::any a;
            is >> std::quoted(tag, '#')
               >> std::quoted(type, '|');
            if
            (   const auto it = td_scan_visitor.find(type)
            ;    it != td_scan_visitor.cend()
            )
                it->second(is, a);
            else
                throw std::runtime_error("sq3p::sq: unregistered type -> " + type);
            _td[tag] = a;
        }
    }
};

    /// A sequence of @a char
    using sq = sq_gen<std::vector<char>>;

// -- stream i/o operators -----------------------------------------------------

    std::ostream& operator<< (std::ostream& os, const sq& s)
    {   s.print(os);
        return os;
    }
    std::istream& operator>> (std::istream& is, sq& s)
    {   s.scan(is);
        return is;
    }

}   // end sq3p namespace

    sq3p::sq operator""_sq (const char* str, std::size_t len)
    {   return sq3p::sq(str);   }

#endif  //_SQ3P_SQ_HPP_