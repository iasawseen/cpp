#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "variant.h"

using namespace std;
using namespace au;


namespace {
    struct helper{
        int tag;
        int& destructor_cnt;
        int& copy_cnt;
        bool throw_on_copy;
        helper(int tag, int &destructor_cnt, int &copy_cnt, bool throw_on_copy = false)
                : tag(tag)
                , destructor_cnt(destructor_cnt)
                , copy_cnt(copy_cnt)
                , throw_on_copy(throw_on_copy)
        {}

        helper(helper&&) = default;

        helper(const helper& other)
            : tag(other.tag)
            , destructor_cnt(other.destructor_cnt)
            , copy_cnt(other.copy_cnt)
            , throw_on_copy(other.throw_on_copy)
        {
            copy_cnt++;
            if (throw_on_copy)
                throw std::bad_alloc();
        }

        virtual ~helper() { destructor_cnt++; }
    };

    struct A {  };
    struct B : A {  };
}


TEST_CASE("Default constructor") {
    variant<int> a;
    CHECK_THROWS(get<int>(a));

    variant<int, double, short> b;
    CHECK_THROWS(get<short>(b));
}


TEST_CASE("Value constructor") {
    variant<int, double> a(5.0);
    CHECK(get<double>(a) == 5.0);
    CHECK_THROWS(get<int>(a));

    variant<int, double> b(5);
    CHECK(get<int>(b) == 5);
    CHECK_THROWS(get<double>(b));

    variant<int, std::string> c("Hello");
    CHECK(get<std::string>(c) == "Hello");
    CHECK_THROWS(get<int>(c));
}


TEST_CASE("Move semantics") {
    int destructor_count = 0;
    int copy_cnt = 0;

    {
        variant<helper> a(std::move(variant<helper>(helper(5, destructor_count, copy_cnt))));
//        variant<helper> a(variant<helper>(helper(5, destructor_count, copy_cnt)));

        variant<helper> b;
        b = std::move(a);
        CHECK(get<helper>(b).tag == 5);
        CHECK_THROWS(get<helper>(a));
    }

    CHECK(copy_cnt == 0);
}


TEST_CASE("get") {
    const variant<int, helper> a(3);
    CHECK(get<int>(a) == 3);

    int* int_ptr = new int(5);
    const variant<int*> b(int_ptr);
    CHECK(*get<int*>(b) == 5);

    variant<int, helper> ax(3);
    CHECK(get<int>(ax) == 3);

    variant<int*> bx(int_ptr);
    CHECK(*get<int*>(bx) == 5);

    delete int_ptr;
}


TEST_CASE("get hirarchy") {
    variant<A, B> x;
    x = B();
    CHECK_THROWS(get<A>(x));
    CHECK_NOTHROW(get<B>(x));
}


TEST_CASE("get by index") {
    variant<int, std::string> a(3);
    CHECK(get<0>(a) == 3);
    const auto aint = a;
    a = "Hello";
    CHECK(get<1>(a) == "Hello");
    const auto astring = a;
    CHECK(get<0>(aint) == 3);
    CHECK(get<1>(astring) == "Hello");
}


TEST_CASE("Check of alignment") {
    struct alignas(128) X {};
    CHECK(alignof(variant<char, X>) == 128);
}


TEST_CASE("test swap") {
    using std::swap;
    variant<int, std::string> a(3);
    variant<int, std::string> b("Hello");
    CHECK(get<std::string>(b) == "Hello");
    CHECK(get<int>(a) == 3);

    swap(a, b);

    CHECK(get<std::string>(a) == "Hello");
    CHECK(get<int>(b) == 3);
}


TEST_CASE("Check destructors") {
    int destructor_count = 0;
    int copy_cnt = 0;
    helper *helper_ptr = new helper(5, destructor_count, copy_cnt);
    {
        variant<helper, int, double> a(helper(5, destructor_count, copy_cnt));
        variant<int, helper*, double> b(helper_ptr);
    }
    CHECK(destructor_count == 2);
    delete helper_ptr;
}


TEST_CASE("Test empty") {
    int destructor_count = 0;
    int copy_cnt = 0;
    variant<int, helper, double> b;
    CHECK(b.empty());
    b = helper(5, destructor_count, copy_cnt);
    CHECK(!b.empty());
}


TEST_CASE("Test clear") {
    int destructor_count = 0;
    int copy_cnt = 0;
    variant<int, helper, double> b(helper(5, destructor_count, copy_cnt));
    CHECK(destructor_count == 1); // Destroying temporary object
    CHECK(!b.empty());
    b.clear();
    CHECK(b.empty());
    CHECK(destructor_count == 2); // Destroying object on clear
}


TEST_CASE("Test which") {
    int destructor_count = 0;
    int copy_cnt = 0;
    variant<int, helper, std::string> b(helper(5, destructor_count, copy_cnt));
    CHECK(b.which() == 1);
    b = 5;
    CHECK(b.which() == 0);
    b = "Hello";
    CHECK(b.which() == 2);
}


//TEST_CASE("Apply visitor") {
//    struct visitor {
//        std::string result;
//
//        void operator()(int a) {
//            result += "int,";
//        }
//        void operator()(std::vector<int> a) {
//            result += "vec,";
//
//        }
//        void operator()(std::string a) {
//            result += "string,";
//        }
//        void operator()() {
//            result += "empty,";
//        }
//    };
//
//    variant<int, std::vector<int>, std::string> a;
//    visitor v;
//    apply_visitor(v, a);
//    a = 5;
//    apply_visitor(v, a);
//    a = vector<int>();
//    apply_visitor(v, a);
//    a = "Hey";
//    apply_visitor(v, a);
//    CHECK(v.result == "empty,int,vec,string,");
//}


void func() {
    static_assert(au::detail::is_convertable<helper, helper>::value, "hi");
}


TEST_CASE("Copy constructor") {
    func();

    int destructor_count = 0;
    int copy_cnt = 0;
    {
        variant<helper> a(helper(5, destructor_count, copy_cnt));
        variant<helper> b(a);
        CHECK(get<helper>(a).tag == 5);
        CHECK(get<helper>(b).tag == 5);
        CHECK(copy_cnt > 0);
    }
    CHECK(destructor_count == 3);
}


TEST_CASE("Copy operator=") {
    variant<int, helper> a(3);
    variant<int, helper> b(5);
    b = a;
    CHECK(get<int>(a) == 3);
    CHECK(get<int>(b) == 3);
    b = variant<int, helper>(6);
    CHECK(get<int>(b) == 6);
}


TEST_CASE("Construct from reference") {
    vector<int> xs(100);
    vector<int> ys(100);

    variant<vector<int>> v(xs);
    vector<int>& ret = get<vector<int>>(v);

    variant<vector<int>> v2;
    v2 = ys;
    vector<int>& ret2 = get<vector<int>>(v);

    CHECK(get<vector<int>>(v).size() == 100);
    CHECK(xs.size() == 100);

    CHECK(get<vector<int>>(v2).size() == 100);
    CHECK(ys.size() == 100);
}


TEST_CASE("get from pointer") {
    vector<int> xs(100);
    variant<vector<int>> v(xs);
    variant<vector<int>>* ptr = &v;

    double* ret_ptr = get<double>(ptr);

    CHECK(ret_ptr == nullptr);
}


TEST_CASE("get pointer from variant") {
    double a = 5.0;
    vector<int> vec(100);

    variant<int*, double*> v(&a);
    variant<int*, vector<int>*> v2(&vec);

    double* ret_ptr = get<double*>(v);
    vector<int>* vec_ptr = get<vector<int>*>(v2);

    *ret_ptr = 7.0;
    *vec_ptr = {1, 2, 3, 4, 5};

    CHECK(*get<double*>(v) == 7.0);
    CHECK(*get<vector<int>*>(v2) ==  vector<int>({1, 2, 3, 4, 5}));
}