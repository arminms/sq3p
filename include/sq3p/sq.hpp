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

#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <any>
#include <initializer_list>
#include <utility>

namespace sq3p {

template <typename Container>
class seq
{   Container                                 _sq;  // sequence
    std::unordered_map<std::string, std::any> _td;  // tagged data

public:
    typedef typename Container::value_type value_type;
    typedef typename Container::size_type size_type;
    typedef typename Container::difference_type difference_type;
    typedef typename Container::reference reference;
    typedef typename Container::const_reference const_reference;
    typedef typename Container::iterator iterator;
    typedef typename Container::const_iterator const_iterator;
    typedef typename Container::reverse_iterator reverse_iterator;
    typedef typename Container::const_reverse_iterator const_reverse_iterator;

    // constructors
    seq() noexcept
    :   _sq()
    ,   _td()
    {}
    explicit seq(std::string sq)
    :   _sq(std::begin(sq), std::end(sq))
    ,   _td()
    {}
    seq(size_type count, const_reference value = value_type(65))
    :   _sq(count, value)
    ,   _td()
    {}
    template<typename InputIt>
    seq(InputIt first, InputIt last)
    :   _sq(first, last)
    ,   _td()
    {}
    seq(const seq& other)
    :   _sq(other._sq)
    ,   _td(other._td)
    {}
    seq(seq&& other)
    :   _sq(std::move(other._sq))
    ,   _td(std::move(other._td))
    {}
    seq(std::initializer_list<value_type> init)
    :   _sq(init)
    ,   _td()
    {}

    // copy assignment operators
    seq& operator= (const seq& other)
    {   _sq = other._sq;
        _td = other._td;
        return *this;
    }
    seq& operator= (seq&& other)
    {   _sq = std::move(other._sq);
        _td = std::move(other._td);
        return *this;
    }
    seq& operator= (std::initializer_list<value_type> init)
    {   _sq = init;
        return *this;
    }

    bool empty() const noexcept
    {   return (_sq.empty() && _td.empty());   }

    // managing tagged data
    bool has(std::string tag) const
    {   return _td.find(tag) == _td.end() ? false : true;  }
    std::any& operator[] (const std::string& tag)
    {   return _td[tag];   }
    std::any& operator[] (std::string&& tag)
    {   return _td[std::forward<std::string>(tag)];   }

    value_type& operator[] (size_type pos)
    {   return _sq[pos];   }

    // comparison operators
    template<typename Container1, typename Container2>
    friend bool operator==
    (   const seq<Container1>& lhs
    ,   const seq<Container2>& rhs
    );
    template<typename Container1, typename Container2>
    friend bool operator!=
    (   const seq<Container1>& lhs
    ,   const seq<Container2>& rhs
    );

    template<template <class> class Format>
    bool load(std::string filename, std::string id, Format<Container> f)
    {   return f(*this, filename, id);   }
};

    template<typename Container1, typename Container2>
    bool operator== (const seq<Container1>& lhs, const seq<Container2>& rhs)
    {   return lhs._sq == rhs._sq;   }

    template<typename Container1, typename Container2>
    bool operator!= (const seq<Container1>& lhs, const seq<Container2>& rhs)
    {   return lhs._sq != rhs._sq;   }

}   // end sq3p namespace

#endif  //_SQ3P_SQ_HPP_