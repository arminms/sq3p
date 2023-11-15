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

namespace sq3p {

template <typename Container>
class seq
{   Container                                 _sq;  // sequence
    std::unordered_map<std::string, std::any> _td;  // tagged data

public:
    seq() {}
    seq(std::string sq)
    {   _sq.resize(std::size(sq));
        std::copy(std::begin(sq), std::end(sq), std::begin(_sq));
    }

    std::any& operator[] (std::string tag)
    {   return _td[tag];   }

    typename Container::value_type& operator[]
    (   typename Container::size_type pos   )
    {   return _sq[pos];   }
};

}   // end sq3p namespace

#endif  //_SQ3P_SQ_HPP_