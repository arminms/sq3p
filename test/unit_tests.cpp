#include <catch2/catch_all.hpp>

#include <sq3p/sq.hpp>
// #include <sq3p/io/fastaq.hpp>

TEMPLATE_TEST_CASE( "sq3p::sq", "[class]", std::vector<char>)
{   typedef TestType T;
    sq3p::seq<T> s{"ACGT"};
    s["test"] = 33;

    SECTION( "comparison operators" )
    {   CHECK(  s == sq3p::seq<T>("ACGT") );
        CHECK(!(s == sq3p::seq<T>("acgt")));
        CHECK(  s != sq3p::seq<T>("acgt") );
        CHECK(!(s != sq3p::seq<T>("ACGT")));
    }

    // constructors
    SECTION( "single value constructor" )
    {   sq3p::seq<T> a4(4);
        CHECK(a4 == sq3p::seq<T>("AAAA"));
        sq3p::seq<T> c4(4, 'C');
        CHECK(c4 == sq3p::seq<T>("CCCC"));
    }
    SECTION( "iterator constructor" )
    {   std::string acgt{"ACGT"};
        sq3p::seq<T> c(std::begin(acgt), std::end(acgt));
        CHECK(s == c);
    }
    SECTION( "copy constructor" )
    {   sq3p::seq<T> c(s);
        CHECK(c == s);
        CHECK(33 == std::any_cast<int>(c["test"]));
    }
    SECTION( "move constructor" )
    {   sq3p::seq<T> m(std::move(s));
        CHECK(s.empty());
        CHECK(m == sq3p::seq<T>("ACGT"));
        CHECK(33 == std::any_cast<int>(m["test"]));
    }
    SECTION( "initializer list" )
    {   sq3p::seq<T> c{'A', 'C', 'G', 'T'};
        CHECK(c == s);
    }

    // copy assignment operators
    SECTION( "copy assignment operator" )
    {   sq3p::seq<T> c = s;
        CHECK(c == s);
        CHECK(33 == std::any_cast<int>(c["test"]));
    }
    SECTION( "move constructor" )
    {   sq3p::seq<T> m = sq3p::seq<T>("ACGT");
        CHECK(m == s);
    }
    SECTION( "initializer list" )
    {   sq3p::seq<T> c = {'A', 'C', 'G', 'T'};
        CHECK(c == s);
    }

    // capacity
    SECTION( "empty()" )
    {   sq3p::seq<T> e;
        CHECK( e.empty() );
        CHECK(!s.empty() );
    }
    SECTION( "size()" )
    {   sq3p::seq<T> e;
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

    SECTION( "tagged data" )
    {   CHECK(s.has("test"));
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

        s["string_literal"] = "hello";
        CHECK(s.has("string_literal"));
        CHECK(0 == std::strcmp
                        (   "hello"
                        ,   std::any_cast<const char*>(s["string_literal"]
                        ) ) );

        s["string"] = std::string("hello");
        CHECK(s.has("string"));
        CHECK("hello" == std::any_cast<std::string>(s["string"]));

        std::vector<int> v{ 1, 2, 3, 4 };
        s["vector_int"] = v;
        CHECK(s.has("vector_int"));
        CHECK(v == std::any_cast<std::vector<int>>(s["vector_int"]));
    }
}
