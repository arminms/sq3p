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
#ifndef _GYNX_IO_FASTAQZ_HPP_
#define _GYNX_IO_FASTAQZ_HPP_

#include <string>
#include <string_view>
#include <stdexcept>
#include <zlib.h>
#include <gynx/io/kseq.h>

namespace gynx::io {

KSEQ_INIT(gzFile, gzread)

template <class Sequence>
struct fast_aqz
{   Sequence operator() (std::string_view filename, size_t ndx)
    {   gzFile fp = filename == "-"
        ?   gzdopen(fileno(stdin), "r")
        :   gzopen(std::string(filename).c_str(), "r");
        if (nullptr == fp)
            throw std::runtime_error
                ("gynx::fast_aqz: could not open file -> " + std::string(filename));
        kseq_t* seq = kseq_init(fp);
        Sequence s;
        size_t count = 0;
        while (kseq_read(seq) >= 0)
            if (ndx == count++)
            {   s = Sequence(seq->seq.s);
                s["_id"] = std::string(seq->name.s);
                if (seq->qual.l)
                    s["_qs"] = std::string(seq->qual.s);
                if (seq->comment.l)
                    s["_desc"] = std::string(seq->comment.s);
                break;
            }
        kseq_destroy(seq);
        gzclose(fp);
        return s;
    }
    Sequence operator() (std::string_view filename, std::string_view id)
    {   gzFile fp = filename == "-"
        ?   gzdopen(fileno(stdin), "r")
        :   gzopen(std::string(filename).c_str(), "r");
        if (nullptr == fp)
            throw std::runtime_error
                ("gynx::fast_aqz: could not open file -> " + std::string(filename));
        kseq_t* seq = kseq_init(fp);
        Sequence s;
        while (kseq_read(seq) >= 0)
        {   std::string name(seq->name.s);
            if (name == id)
            {   s = Sequence(seq->seq.s);
                s["_id"] = name;
                if (seq->qual.l)
                    s["_qs"] = std::string(seq->qual.s);
                if (seq->comment.l)
                    s["_desc"] = std::string(seq->comment.s);
                break;
            } 
        }
        kseq_destroy(seq);
        gzclose(fp);
        return s;
    }
};

}   // end gynx::io namespace

#endif  //_GYNX_IO_FASTAQZ_HPP_