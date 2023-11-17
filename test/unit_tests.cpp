#include <cstring>

#include <catch2/catch_all.hpp>
#include <sq3p/sq.hpp>

TEMPLATE_TEST_CASE( "sq3p::sq", "[class]", std::vector<char>)
{   typedef TestType T;
    sq3p::seq<T> s{"ACGT"};
    s["test"] = 33;

    SECTION( "comparison operator" )
    {   CHECK(sq3p::seq<T>("ACGT") == s);
        CHECK(!(sq3p::seq<T>("acgt") == s));
    }

    // constructors
    SECTION( "single value constructor" )
    {   sq3p::seq<T> a4(4);
        CHECK(sq3p::seq<T>("AAAA") == a4);
        sq3p::seq<T> c4(4, 'C');
        CHECK(sq3p::seq<T>("CCCC") == c4);
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
        CHECK('A' == m[0]);
        CHECK('C' == m[1]);
        CHECK('G' == m[2]);
        CHECK('T' == m[3]);
        CHECK(33 == std::any_cast<int>(m["test"]));
    }
    SECTION( "initializer list" )
    {   sq3p::seq<T> c{'A', 'C', 'G', 'T'};
        CHECK(c == s);
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
        CHECK(0 == std::strcmp("hello", std::any_cast<const char*>(s["string_literal"])));

        s["string"] = std::string("hello");
        CHECK(s.has("string"));
        CHECK("hello" == std::any_cast<std::string>(s["string"]));

        std::vector<int> v{ 1, 2, 3, 4 };
        s["vector_int"] = v;
        CHECK(s.has("vector_int"));
        CHECK(v == std::any_cast<std::vector<int>>(s["vector_int"]));
    }
}
