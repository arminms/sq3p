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
#ifndef _GYNX_SQ_HPP_
#define _GYNX_SQ_HPP_

#include <sstream>
#include <algorithm>
#include <initializer_list>
#include <stdexcept>

#include <gynx/visitor.hpp>

namespace gynx {

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

// -- iterators ----------------------------------------------------------------

    ///
    /// Returns a read/write iterator that points to the first residue in the
    /// @a sq. Iteration is done in ordinary residue order.
    iterator begin() noexcept
    {   return _sq.begin();   }
    ///
    /// Returns a read-only (constant) iterator that points to the first
    /// residue in the @a sq. Iteration is done in ordinary residue order.
    const_iterator begin() const noexcept
    {   return _sq.begin();   }
    ///
    /// Returns a read-only (constant) iterator that points to the first
    /// residue in the @a sq. Iteration is done in ordinary residue order.
    const_iterator cbegin() const noexcept
    {   return _sq.cbegin();   }
    ///
    /// Returns a read/write iterator that points one past the last residue in
    /// the @a sq. Iteration is done in ordinary residue order.
    iterator end() noexcept
    {   return _sq.end();   }
    ///
    /// Returns a read-only (constant) iterator that points one past the last
    /// residue in the @a sq. Iteration is done in ordinary residue order.
    const_iterator end() const noexcept
    {   return _sq.end();   }
    ///
    /// Returns a read-only (constant) iterator that points one past the last
    /// residue in the @a sq. Iteration is done in ordinary residue order.
    const_iterator cend() const noexcept
    {   return _sq.cend();   }
    ///
    /// Returns a read/write iterator that points to the last residue in the
    /// @a sq. Iteration is done in reverse residue order.
    reverse_iterator rbegin() noexcept
    {   return _sq.rbegin();   }
    ///
    /// Returns a read-only (constant) iterator that points to the last residue
    /// in the @a sq. Iteration is done in reverse residue order.
    const_reverse_iterator rbegin() const noexcept
    {   return _sq.rbegin();   }
    ///
    /// Returns a read-only (constant) iterator that points to the last residue
    /// in the @a sq. Iteration is done in reverse residue order.
    const_reverse_iterator crbegin() const noexcept
    {   return _sq.crbegin();   }
    ///
    /// Returns a read/write iterator that points to one before the first
    /// residue in the @a sq. Iteration is done in reverse residue order.
    reverse_iterator rend() noexcept
    {   return _sq.rend();   }
    ///
    /// Returns a read-only (constant) iterator that points to one before the
    /// first residue in the @a sq. Iteration is done in reverse residue order.
    const_reverse_iterator rend() const noexcept
    {   return _sq.rend();   }
    ///
    /// Returns a read-only (constant) iterator that points to one before the
    /// first residue in the @a sq. Iteration is done in reverse residue order.
    const_reverse_iterator crend() const noexcept
    {   return _sq.crend();   }

// -- capacity -----------------------------------------------------------------

    /// Returns true if the @a sq is empty. (Thus begin() would equal end().)
    bool empty() const noexcept
    {   return (_sq.empty() && _td.empty());   }
    size_type size() const noexcept
    {   return _sq.size();   }

// -- subscript operator -------------------------------------------------------

    reference operator[] (size_type pos)
    {   return _sq[pos];   }
    const_reference operator[] (size_type pos) const
    {   return _sq[pos];   }

// -- subseq operator ----------------------------------------------------------

    sq_gen operator() (size_type pos, size_type count = std::string::npos) const
    {   if (pos > _sq.size())
            throw std::out_of_range("gynx::sq: pos > this->size()");
        return sq_gen
        (   _sq.begin() + pos
        ,   (count > _sq.size() - pos) ? _sq.end() : _sq.begin() + pos + count
        );
    }

// -- managing tagged data -----------------------------------------------------

    bool has(std::string tag) const
    {   return _td.find(tag) == _td.end() ? false : true;  }
    std::any& operator[] (const std::string& tag)
    {   return _td[tag];   }
    std::any& operator[] (std::string&& tag)
    {   return _td[std::forward<std::string>(tag)];   }

// -- comparison operators -----------------------------------------------------

    template<typename Container1, typename Container2>
    friend
    bool operator== (const sq_gen<Container1>& lhs, const sq_gen<Container2>& rhs)
    {   return lhs._sq == rhs._sq;   }
    template<typename Container1, typename Container2>
    friend
    bool operator!= (const sq_gen<Container1>& lhs, const sq_gen<Container2>& rhs)
    {   return lhs._sq != rhs._sq;   }

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
                throw std::runtime_error("gynx::sq: unregistered type -> " + type);
            _td[tag] = a;
        }
    }
};

// -- i/o stream operators -----------------------------------------------------

    template<typename T>
    std::ostream& operator<< (std::ostream& os, const sq_gen<T>& s)
    {   s.print(os);
        return os;
    }
    template<typename T>
    std::istream& operator>> (std::istream& is, sq_gen<T>& s)
    {   s.scan(is);
        return is;
    }

    ///
    /// A sequence of @a char
    using sq = sq_gen<std::vector<char>>;

}   // end gynx namespace

// -- string literal operator --------------------------------------------------

    gynx::sq operator""_sq (const char* str, std::size_t len)
    {   return gynx::sq(str);   }

#endif  //_GYNX_SQ_HPP_