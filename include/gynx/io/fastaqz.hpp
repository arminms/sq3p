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

/// @brief A function object for reading FASTA/FASTQ files (possibly compressed
/// with gzip) and loading sequences into the specified @a Sequence type.
/// @tparam Sequence
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
        size_t count = 0;
        int r{};
        while (r = kseq_read(seq) >= 0)
            if (ndx == count++)
                break;
        Sequence s = (r > 0) ? Sequence(seq->seq.s) : Sequence();
        if (r > 0)
        {   s["_id"] = std::string(seq->name.s);
            if (seq->qual.l)
                s["_qs"] = std::string(seq->qual.s);
            if (seq->comment.l)
                s["_desc"] = std::string(seq->comment.s);
        }
        kseq_destroy(seq);
        gzclose(fp);
        if (-2 == r)
            throw std::runtime_error
                ("gynx::fast_aqz: truncated quality string in file -> " + std::string(filename));
        if (-3 == r)
            throw std::runtime_error
                ("gynx::fast_aqz: error reading file -> " + std::string(filename)); 
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
        int r{};
        while (r = kseq_read(seq) >= 0)
        {   std::string_view name(seq->name.s);
            if (name == id)
                break;
        }
        Sequence s = (r > 0) ? Sequence(seq->seq.s) : Sequence();
        if (r > 0)
        {   s["_id"] = std::string(seq->name.s);
            if (seq->qual.l)
                s["_qs"] = std::string(seq->qual.s);
            if (seq->comment.l)
                s["_desc"] = std::string(seq->comment.s);
        }
        kseq_destroy(seq);
        gzclose(fp);
        if (-2 == r)
            throw std::runtime_error
                ("gynx::fast_aqz: truncated quality string in file -> " + std::string(filename));
        if (-3 == r)
            throw std::runtime_error
                ("gynx::fast_aqz: error reading file -> " + std::string(filename)); 
        return s;
    }
};

struct fasta_gz
{   template <class Sequence>
    int operator() (std::string_view filename, const Sequence& seq)
    {   gzFile fp = filename == "-"
        ?   gzdopen(fileno(stdout), "w")
        :   gzopen(std::string(filename).c_str(), "w");
        if (nullptr == fp)
            throw std::runtime_error
                ("gynx::fasta: could not open file -> " + std::string(filename));
        std::string id = seq.has("_id")
        ?   std::any_cast<std::string>(seq["_id"])
        :   "seq";
        std::string desc = seq.has("_desc")
        ?   " " + std::any_cast<std::string>(seq["_desc"])
        :   "";
        std::string header = ">" + id + desc + "\n";
        gzwrite(fp, header.c_str(), header.size());
        gzwrite(fp, seq.data(), seq.size());
        gzwrite(fp, "\n", 1);
        gzclose(fp);
        return 0;
    }
};

}   // end gynx::io namespace

#endif  //_GYNX_IO_FASTAQZ_HPP_