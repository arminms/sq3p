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
#ifndef _SQ3P_IO_FASTAQZ_HPP_
#define _SQ3P_IO_FASTAQZ_HPP_

#include <zlib.h>
#include <sq3p/io/kseq.h>

namespace sq3p::in {

KSEQ_INIT(gzFile, gzread)

template <typename Container>
struct fastaqz
{   bool operator()
    (   sq3p::seq<Container>& s
    ,   std::string filename
    ,   std::string id) const
    {   gzFile fp = filename == "-"
        ?   gzdopen(fileno(stdin), "r")
        :   gzopen(filename.c_str(), "r");
        if (nullptr == fp)
            throw std::runtime_error
                ("sq3p::faqz: could not open file -> " + filename);
        kseq_t* seq = kseq_init(fp);
        while (kseq_read(seq) >= 0)
        {   std::string name(seq->name.s);
            if (name == id)
            {   sq3p::seq<Container> in(seq->seq.s);
                in["_id"] = name;
                if (seq->qual.l)
                    in["_qs"] = std::string(seq->qual.s);
                if (seq->comment.l)
                    in["_desc"] = std::string(seq->comment.s);
                s = in;
                break;
            } 
        }
        kseq_destroy(seq);
        gzclose(fp);
        return s.has("_id");
    }
};

    using faqz = fastaqz<std::vector<char>>;

}   // end sq3p::in namespace

#endif  //_SQ3P_IO_FASTAQZ_HPP_