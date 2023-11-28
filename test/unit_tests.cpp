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
#include <catch2/catch_all.hpp>

#include <sq3p/sq.hpp>
#include <sq3p/io/faqz.hpp>

TEMPLATE_TEST_CASE( "sq3p::sq", "[class]", std::vector<char>)
{   typedef TestType T;
    sq3p::sq_gen<T> s{"ACGT"};
    s["test-int"] = -33;

    SECTION( "comparison operators" )
    {   REQUIRE(  s == sq3p::sq_gen<T>("ACGT") );
        REQUIRE(!(s == sq3p::sq_gen<T>("acgt")));
        REQUIRE(  s != sq3p::sq_gen<T>("acgt") );
        REQUIRE(!(s != sq3p::sq_gen<T>("ACGT")));
    }

    // constructors
    SECTION( "single value constructor" )
    {   sq3p::sq_gen<T> a4(4);
        CHECK(a4 == sq3p::sq_gen<T>("AAAA"));
        sq3p::sq_gen<T> c4(4, 'C');
        CHECK(c4 == sq3p::sq_gen<T>("CCCC"));
    }
    SECTION( "iterator constructor" )
    {   std::string acgt{"ACGT"};
        sq3p::sq_gen<T> c(std::begin(acgt), std::end(acgt));
        CHECK(s == c);
    }
    SECTION( "copy constructor" )
    {   sq3p::sq_gen<T> c(s);
        CHECK(c == s);
        CHECK(-33 == std::any_cast<int>(c["test-int"]));
    }
    SECTION( "move constructor" )
    {   sq3p::sq_gen<T> m(std::move(s));
        CHECK(s.empty());
        CHECK(m == sq3p::sq_gen<T>("ACGT"));
        CHECK(-33 == std::any_cast<int>(m["test-int"]));
    }
    SECTION( "initializer list" )
    {   sq3p::sq_gen<T> c{'A', 'C', 'G', 'T'};
        CHECK(c == s);
    }

    // copy assignment operators
    SECTION( "copy assignment operator" )
    {   sq3p::sq_gen<T> c = s;
        CHECK(c == s);
        CHECK(-33 == std::any_cast<int>(c["test-int"]));
    }
    SECTION( "move constructor" )
    {   sq3p::sq_gen<T> m = sq3p::sq_gen<T>("ACGT");
        CHECK(m == s);
    }
    SECTION( "initializer list" )
    {   sq3p::sq_gen<T> c = {'A', 'C', 'G', 'T'};
        CHECK(c == s);
    }

    // capacity
    SECTION( "empty()" )
    {   sq3p::sq_gen<T> e;
        CHECK( e.empty() );
        CHECK(!s.empty() );
    }
    SECTION( "size()" )
    {   sq3p::sq_gen<T> e;
        CHECK(0 == e.size());
        CHECK(4 == s.size());
    }

    SECTION( "subscript/array index operator" )
    {   CHECK('A' == s[0]);
        CHECK('C' == s[1]);
        CHECK('G' == s[2]);
        CHECK('T' == s[3]);
        s[3] = 'U';
        CHECK('U' == s[3]);
    }

    SECTION( "subseq operator" )
    {   sq3p::sq_gen<T> org{"CCATACGTGAC"};
        CHECK(org(4, 4) == s);
        CHECK(org(0) == org);
        CHECK(org(4) == sq3p::sq_gen<T>{"ACGTGAC"});
        CHECK_THROWS_AS(org(20) == sq3p::sq_gen<T>{"ACGTGAC"}, std::out_of_range);
    }

    SECTION( "tagged data" )
    {   CHECK(s.has("test-int"));
        CHECK(false == s.has("no"));

        s["int"] = 19;
        CHECK(s.has("int"));
        CHECK(19 == std::any_cast<int>(s["int"]));

        s["float"] = 3.14f;
        CHECK(s.has("float"));
        CHECK(3.14f == std::any_cast<float>(s["float"]));

        s["double"] = 3.14;
        CHECK(s.has("double"));
        CHECK(3.14 == std::any_cast<double>(s["double"]));

        s["string"] = std::string("hello");
        CHECK(s.has("string"));
        CHECK("hello" == std::any_cast<std::string>(s["string"]));

        std::vector<int> v{ 1, 2, 3, 4 };
        s["vector_int"] = v;
        CHECK(s.has("vector_int"));
        CHECK(v == std::any_cast<std::vector<int>>(s["vector_int"]));
    }

    SECTION( "input/output operators")
    {   s["test-void"] = {};
        s["test-bool"] = true;
        s["test-unsigned"] = 33u;
        s["test-float"] = 3.14f;
        s["test-double"] = 3.14;
        s["test-string"] = std::string("hello");
        s["test-vector-int"] = std::vector<int>{ 1, 2, 3, 4 };

        std::stringstream ss;
        ss << s;
        sq3p::sq_gen<T> t;
        ss >> t;

        CHECK(s == t);
        CHECK(s.has("test-void"));
        CHECK(t.has("test-void"));
        CHECK(std::any_cast<bool>(s["test-bool"]) == std::any_cast<bool>(t["test-bool"]));
        CHECK(std::any_cast<int>(s["test-int"]) == std::any_cast<int>(t["test-int"]));
        CHECK(std::any_cast<unsigned>(s["test-unsigned"]) == std::any_cast<unsigned>(t["test-unsigned"]));
        CHECK(std::any_cast<float>(s["test-float"]) == std::any_cast<float>(t["test-float"]));
        CHECK(std::any_cast<double>(s["test-double"]) == std::any_cast<double>(t["test-double"]));
        CHECK(std::any_cast<std::string>(s["test-string"]) == std::any_cast<std::string>(t["test-string"]));
        CHECK(4 == std::any_cast<std::vector<int>>(s["test-vector-int"]).size());
    }

    SECTION( "string literal operator" )
    {   auto t = "ACGT"_sq;
        CHECK(s == t);
        CHECK(s == "ACGT"_sq);
    }
}

TEMPLATE_TEST_CASE( "sq3p::in::faqz", "[io][in]", std::vector<char>)
{   typedef TestType T;
    sq3p::sq_gen<T> s;
    CHECK_THROWS_AS
    (   s.load("wrong.fa", "no_id", sq3p::in::faqz_gen<T>() )
    ,   std::runtime_error
    );
    CHECK(false == s.load(SAMPLE_GENOME, "bad_id",sq3p::in::faqz_gen<T>()));
    CHECK(s.load(SAMPLE_GENOME, "NC_017288.1",sq3p::in::faqz_gen<T>()));
    CHECK(7553 == std::size(s));
    CHECK(s(0, 10) == sq3p::sq_gen<T>{"TATAATTAAA"});
    CHECK(s (7543) == sq3p::sq_gen<T>{"TCCAATTCTA"});
    CHECK("NC_017288.1" == std::any_cast<std::string>(s["_id"]));
    std::string desc("Chlamydia psittaci 6BC plasmid pCps6BC, complete sequence");
    CHECK(desc == std::any_cast<std::string>(s["_desc"]));
}
