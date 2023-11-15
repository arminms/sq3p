#include <cstring>

#include <catch2/catch_all.hpp>
#include <sq3p/sq.hpp>

TEMPLATE_TEST_CASE( "sq3p::sq", "[class]", std::vector<char>)
{   typedef TestType T;

    sq3p::seq<T> s{"ACGT"};
    CHECK('A' == s[0]);
    CHECK('C' == s[1]);
    CHECK('G' == s[2]);
    CHECK('T' == s[3]);

    s["int"] = 19;
    CHECK(19 == std::any_cast<int>(s["int"]));

    s["float"] = 3.14f;
    CHECK(3.14f == std::any_cast<float>(s["float"]));

    s["double"] = 3.14;
    CHECK(3.14 == std::any_cast<double>(s["double"]));

    s["string_literal"] = "hello";
    CHECK(0 == std::strcmp("hello", std::any_cast<const char*>(s["string_literal"])));

    s["string"] = std::string("hello");
    CHECK("hello" == std::any_cast<std::string>(s["string"]));

    std::vector<int> v{ 1, 2, 3, 4 };
    s["vector_int"] = v;
    CHECK(v == std::any_cast<std::vector<int>>(s["vector_int"]));
}
