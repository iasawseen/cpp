//#define TESTS

#ifdef TESTS
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#endif

#include "graph.h"
#include "shared_graph.h"

#include "filtered_graph.h"
#include "shortest_path.h"

#include <iostream>
#include <unordered_set>
#include <vector>
#include <typeinfo>
#include <cassert>

using namespace std;
using namespace au;

#define TEST_UNIT1
#define TEST_UNIT2
#define TEST_UNIT3
#define TEST_UNIT4
#define TEST_UNIT5
#define TEST_UNIT6
#define TEST_UNIT7
#define TEST_UNIT8
#define TEST_UNIT9
#define TEST_UNIT10
#define TEST_UNIT11
#define TEST_UNIT12
#define TEST_UNIT13

#ifdef TESTS


TEST_CASE("add_vertex_test") {
#ifdef TEST_UNIT1
    graph<std::string, int> gh;
    std::unordered_set<std::string> strings = {"hi", "bye"};

    for (auto it : strings) {
        gh.add_vertex(it);
    }

    std::unordered_set<std::string> results;

    for (auto iter = gh.vertex_begin(); iter != gh.vertex_end(); ++iter) {
        results.emplace(*iter);
    }

    CHECK(strings == results);

#endif // TEST_UNIT1
}

TEST_CASE("find_vertex_test") {
#ifdef TEST_UNIT2
    graph<std::string, int> gh;
    std::unordered_set<std::string> strings = {"hi", "bye"};

    for (auto it : strings) {
        gh.add_vertex(it);
    }

    auto it = gh.find_vertex("hi");

    CHECK(*it == "hi");

#endif // TEST_UNIT2
}

TEST_CASE("remove_vertex_test") {
#ifdef TEST_UNIT3
    graph<std::string, int> gh;
    std::unordered_set<std::string> strings = {"hi", "bye"};
    std::unordered_set<std::string> results = {"bye"};

    for (auto it : strings) {
        gh.add_vertex(it);
    }

    auto it = gh.find_vertex("hi");

    gh.remove_vertex(it);

    std::unordered_set<std::string> actuals;

    for (auto iter = gh.vertex_begin(); iter != gh.vertex_end(); ++iter) {
        actuals.emplace(*iter);
    }
    CHECK(results == actuals);

#endif // TEST_UNIT3
}

TEST_CASE("const_test") {
#ifdef TEST_UNIT4
    graph<std::string, int> gh;
    std::unordered_set<std::string> strings = {"hi", "bye"};

    for (auto it : strings) {
        gh.add_vertex(it);
    }

    graph<std::string, int> const cp = gh;

    std::unordered_set<std::string> actuals;

    for (auto iter = cp.vertex_begin(); iter != cp.vertex_end(); ++iter) {
        actuals.emplace(*iter);

    }

    CHECK(strings == actuals);

#endif // TEST_UNIT4
}

TEST_CASE("add_edge_test") {
#ifdef TEST_UNIT5
    graph<std::string, int> gh;

    auto from = gh.add_vertex("from");
    auto to = gh.add_vertex("to");

    gh.add_edge(from, to, 10);

#endif // TEST_UNIT5
}

TEST_CASE("find_edge_test") {
#ifdef TEST_UNIT6
    graph<std::string, int> gh;

    std::string from_str = "from";
    std::string to_str = "to";

    auto from = gh.add_vertex(from_str);
    auto to = gh.add_vertex(to_str);

    gh.add_edge(from, to, 10);
    auto it = gh.find_edge(from, to);

    CHECK(*(it.from()) == from_str);
    CHECK(*(it.to()) == to_str);

#endif // TEST_UNIT6
}

TEST_CASE("multiple_add_edge_test") {
#ifdef TEST_UNIT7
    graph<std::string, int> gh;

    std::vector<std::string> from_strs = {"from"};
    std::vector<std::string> to_strs = {"to1", "to2", "to3", "to4", "to5", "to6"};

    for (size_t i = 0; i < from_strs.size(); ++i) {
        gh.add_vertex(from_strs[i]);
    }

    for (size_t i = 0; i < to_strs.size(); ++i) {
        gh.add_vertex(to_strs[i]);
    }


    auto from = gh.find_vertex(from_strs[0]);

    std::unordered_set<int> numbers;
    std::unordered_set<int> results;


    for (size_t i = 0; i < 6; ++i) {
        auto to = gh.find_vertex(to_strs[i]);
        numbers.insert(i);
        gh.add_edge(from, to, i);
    }

    for (auto it = gh.edge_begin(from); it != gh.edge_end(from); ++it) {
        results.insert(*it);
    }

    CHECK(numbers == results);

#endif // TEST_UNIT7
}

TEST_CASE("remove_edge_test") {
#ifdef TEST_UNIT8
    graph<std::string, int> gh;

    std::vector<std::string> from_strs = {"from"};
    std::vector<std::string> to_strs = {"to1", "to2", "to3", "to4", "to5", "to6"};

    for (size_t i = 0; i < from_strs.size(); ++i) {
        gh.add_vertex(from_strs[i]);
    }

    for (size_t i = 0; i < to_strs.size(); ++i) {
        gh.add_vertex(to_strs[i]);
    }


    auto from = gh.find_vertex(from_strs[0]);

    std::unordered_set<int> results = {1, 3, 5};
    std::unordered_set<int> actuals;

    for (size_t i = 0; i < 6; ++i) {
        auto to = gh.find_vertex(to_strs[i]);
        gh.add_edge(from, to, i + 1);
    }

    auto to2 = gh.find_vertex(to_strs[1]);
    auto edge_to2 = gh.find_edge(from, to2);
    gh.remove_edge(edge_to2);

    auto to4 = gh.find_vertex(to_strs[3]);
    auto edge_to4 = gh.find_edge(from, to4);
    gh.remove_edge(edge_to4);

    auto to6 = gh.find_vertex(to_strs[5]);
    auto edge_to6 = gh.find_edge(from, to6);
    gh.remove_edge(edge_to6);

    for (auto it = gh.edge_begin(from); it != gh.edge_end(from); ++it) {
        actuals.insert(*it);
    }

    CHECK(actuals == results);

#endif // TEST_UNIT8
}

TEST_CASE("remove_vertex_edge_test") {
#ifdef TEST_UNIT9
    graph<std::string, int> gh;

    std::vector<std::string> from_strs = {"from1", "from2"};
    std::vector<std::string> middle_strs = {"mid1", "mid2", "mid3"};
    std::vector<std::string> to_strs = {"to1", "to2", "to3", "to4", "to5", "to6"};


    for (size_t i = 0; i < from_strs.size(); ++i) {
        gh.add_vertex(from_strs[i]);
    }

    for (size_t i = 0; i < to_strs.size(); ++i) {
        gh.add_vertex(to_strs[i]);
    }

    for (size_t i = 0; i < middle_strs.size(); ++i) {
        gh.add_vertex(middle_strs[i]);
    }

    // connect all froms to all middles

    for (auto from : from_strs) {
        auto from_iter = gh.find_vertex(from);
        for (auto mid : middle_strs) {
            auto mid_iter = gh.find_vertex(mid);
            auto it = gh.add_edge(from_iter, mid_iter, 5);
        }
    }

    // connect all middles to all tos

    for (auto middle : middle_strs) {
        auto middle_iter = gh.find_vertex(middle);
        for (auto to : to_strs) {
            auto to_iter = gh.find_vertex(to);
            auto it = gh.add_edge(middle_iter, to_iter, 10);
        }
    }

    auto from = gh.find_vertex("from1");
    auto mid1 = gh.find_vertex("mid1");
    auto to = gh.find_vertex("to1");
    auto it = gh.find_edge(from, to);

    CHECK(gh.find_edge(from, to) == gh.edge_end(from));
    CHECK(gh.find_edge(from, mid1) != gh.edge_end(from));
    CHECK(gh.find_edge(mid1, to) != gh.edge_end(mid1));

    gh.remove_vertex(mid1);

    auto from1 = gh.find_vertex("from1");
    auto from2 = gh.find_vertex("from2");

    auto mid2 = gh.find_vertex("mid2");
    auto mid3 = gh.find_vertex("mid3");

    auto to1 = gh.find_vertex("to1");
    auto to2 = gh.find_vertex("to2");
    auto to3 = gh.find_vertex("to3");
    auto to4 = gh.find_vertex("to4");
    auto to5 = gh.find_vertex("to5");
    auto to6 = gh.find_vertex("to6");

    CHECK(gh.find_edge(from1, mid2) != gh.edge_end(from1));
    CHECK(gh.find_edge(from2, mid2) != gh.edge_end(from2));
    CHECK(gh.find_edge(from1, mid3) != gh.edge_end(from1));
    CHECK(gh.find_edge(from2, mid3) != gh.edge_end(from2));

    CHECK(gh.find_edge(mid2, to1) != gh.edge_end(mid2));
    CHECK(gh.find_edge(mid2, to2) != gh.edge_end(mid2));
    CHECK(gh.find_edge(mid2, to3) != gh.edge_end(mid2));
    CHECK(gh.find_edge(mid2, to4) != gh.edge_end(mid2));
    CHECK(gh.find_edge(mid2, to5) != gh.edge_end(mid2));
    CHECK(gh.find_edge(mid2, to6) != gh.edge_end(mid2));

    CHECK(gh.find_edge(mid3, to1) != gh.edge_end(mid3));
    CHECK(gh.find_edge(mid3, to2) != gh.edge_end(mid3));
    CHECK(gh.find_edge(mid3, to3) != gh.edge_end(mid3));
    CHECK(gh.find_edge(mid3, to4) != gh.edge_end(mid3));
    CHECK(gh.find_edge(mid3, to5) != gh.edge_end(mid3));
    CHECK(gh.find_edge(mid3, to6) != gh.edge_end(mid3));

#endif // TEST_UNIT9
}


struct vertex_fl_10 {
    template<class T>
    bool operator()(T const& t) const {
        if (t < 10) {
            return true;
        }
        return false;
    }
};

TEST_CASE("filter_graph_vertices_test") {
#ifdef TEST_UNIT10
    graph<int, int> gh;

    std::vector<int> vertices = {1, 5, 9, 11, 4};
    std::unordered_set<int> filtered = {1, 5, 9, 4};
    std::unordered_set<int> actuals;


    for (size_t i = 0; i < vertices.size(); ++i) {
        gh.add_vertex(vertices[i]);
    }

    filtered_graph<graph<int, int>, vertex_fl_10, vertex_fl_10> fl(gh, vertex_fl_10(), vertex_fl_10());

    for(auto it = fl.vertex_begin(); it != fl.vertex_end(); ++it) {
        actuals.insert(*it);
    }

    CHECK(filtered == actuals);

#endif // TEST_UNIT10
}

TEST_CASE("filter_graph_edges_test") {
#ifdef TEST_UNIT11
    graph<int, int> gh;

    int from11 = 11;
    int from5 = 5;
    int to4 = 4;
    int to12 = 12;

    std::vector<int> tos = {0, 1, 2, 12, 4, 13, 14};
    std::unordered_set<int> filtered = {};
    std::unordered_set<int> actuals;
    std::unordered_set<int> filtered2 = {0, 2, 4, 8};
    std::unordered_set<int> actuals2;

    gh.add_vertex(from11);
    gh.add_vertex(from5);

    for (size_t i = 0; i < tos.size(); ++i) {
        gh.add_vertex(tos[i]);
    }

    auto from_it = gh.find_vertex(from11);
    auto from_it2 = gh.find_vertex(from5);

    for (size_t i = 0; i < tos.size(); ++i) {
        auto to_it = gh.find_vertex(tos[i]);
        gh.add_edge(from_it, to_it, i * 2);
        gh.add_edge(from_it2, to_it, i * 2);
    }

//    from-edge-to
//    5/11 - 0 - 0
//    5/11 - 2 - 1
//    5/11 - 4 - 2
//    5/11 - 6 - 12
//    5/11 - 8 - 4
//    5/11 -10 -13
//    5/11 -12 -14

    filtered_graph<graph<int, int>, vertex_fl_10, vertex_fl_10> fl(gh, vertex_fl_10(), vertex_fl_10());

    auto from_it_fl = fl.find_vertex(from11);
    auto from_it_fl2 = fl.find_vertex(from5);

    for(auto it = fl.edge_begin(from_it_fl); it != fl.edge_end(from_it_fl); ++it) {
        actuals.insert(*it);
    }

    CHECK(filtered == actuals);

    for(auto it = fl.edge_begin(from_it_fl2); it != fl.edge_end(from_it_fl2); ++it) {
        actuals2.insert(*it);
    }

    CHECK(filtered2 == actuals2);

    auto from5_ = fl.find_vertex(from5);
    auto to4_ = fl.find_vertex(to4);
    auto to12_ = fl.find_vertex(to12);

    CHECK(to12_ == fl.vertex_end());
    CHECK(8 == *fl.find_edge(from5_, to4_));
    CHECK(fl.find_edge(from5_, to12_) == fl.edge_end(from5_));

#endif // TEST_UNIT11
}

struct edge_len {
    template<class edge_iter>
    double operator()(edge_iter const& e) {
        return static_cast<double>(e);
    }
};

struct path_visitor {
    template<class edge_iter>
    void operator()(edge_iter const& e) {
        path.push_back(static_cast<double>(*e));
    }

    std::vector<double> path;
};


TEST_CASE("shortest_path") {
#ifdef TEST_UNIT12
    graph<int, int> gh;

    for (int i = 1; i <= 6; ++i) {
        gh.add_vertex(i);
    }
    auto v1 = gh.find_vertex(1);
    auto v2 = gh.find_vertex(2);
    auto v3 = gh.find_vertex(3);
    auto v4 = gh.find_vertex(4);
    auto v5 = gh.find_vertex(5);

    gh.add_edge(v1, v2, 1);
    gh.add_edge(v1, v3, 15);
    gh.add_edge(v1, v4, 10);

    gh.add_edge(v2, v3, 5);
    gh.add_edge(v2, v4, 2);

    gh.add_edge(v3, v5, 11);

    gh.add_edge(v4, v5, 3);

    const int from = 1;
    const int to = 5;
    const int end_to = 6;

    auto v1_ = gh.find_vertex(from);
    auto v5_ = gh.find_vertex(to);
    auto v6_ = gh.find_vertex(end_to);

    auto visitor = path_visitor();

    bool has_path = find_shortest_path(gh, v1_, v5_, edge_len(), visitor);

    std::vector<double> best_path = {1, 2, 3};

    CHECK(has_path);
    CHECK(best_path == visitor.path);
    CHECK(!find_shortest_path(gh, v1_, v6_, edge_len(), visitor));

#endif // TEST_UNIT12
}

graph<int, int> func() {
    graph<int, int> tmp;
    for (int i = 1; i <= 5; ++i) {
        tmp.add_vertex(i);
    }
    auto v1 = tmp.find_vertex(1);
    auto v2 = tmp.find_vertex(2);
    auto v3 = tmp.find_vertex(3);
    auto v4 = tmp.find_vertex(4);
    auto v5 = tmp.find_vertex(5);

    tmp.add_edge(v1, v2, 1);
    tmp.add_edge(v1, v3, 15);
    tmp.add_edge(v1, v4, 10);

    return tmp;
}

TEST_CASE("graph_copy_test") {
#ifdef TEST_UNIT13
    graph<int, int> gh;

    for (int i = 1; i <= 5; ++i) {
        gh.add_vertex(i);
    }
    auto v1 = gh.find_vertex(1);
    auto v2 = gh.find_vertex(2);
    auto v3 = gh.find_vertex(3);
    auto v4 = gh.find_vertex(4);
    auto v5 = gh.find_vertex(5);

    gh.add_edge(v1, v2, 1);
    gh.add_edge(v1, v3, 15);
    gh.add_edge(v1, v4, 10);

    gh.add_edge(v2, v3, 5);
    gh.add_edge(v2, v4, 2);

    gh.add_edge(v3, v5, 11);

    gh.add_edge(v4, v5, 3);

    graph<int, int> copy1(gh);
    graph<int, int> copy2 = gh;
    graph<int, int> copy3(func());
    graph<int, int> copy4(gh);
    graph<int, int> copy5(std::move(copy4));
    graph<int, int> copy6 = std::move(copy5);


    CHECK(copy3.find_vertex(1) != copy3.vertex_end());
    CHECK(1 == *copy3.find_edge(copy3.find_vertex(1), copy3.find_vertex(2)));

    const int from = 1;
    const int to = 5;
    const int mid = 2;

    CHECK(from == *gh.find_vertex(from));
    CHECK(from == *copy1.find_vertex(from));
    CHECK(from == *copy2.find_vertex(from));

    gh.remove_vertex(gh.find_vertex(mid));

    CHECK(gh.find_vertex(mid) == gh.vertex_end());
    CHECK(copy1.find_vertex(mid) != copy1.vertex_end());
    CHECK(copy2.find_vertex(mid) != copy2.vertex_end());

    copy1.find_edge(copy1.find_vertex(mid), copy1.find_vertex(to));

//    auto v1_ = gh.find_vertex(from);
//    auto v5_ = gh.find_vertex(to);


#endif // TEST_UNIT13
}

#endif // TESTS

#ifndef TESTS

#include <cassert>
#include <type_traits>
#include <iterator>
#include <iostream>
#include <vector>

#include <cassert>
#include <type_traits>
#include <iterator>
#include <iostream>
#include <vector>



template<class T>
using is_const_ref = std::is_const<std::remove_reference_t<T>>;

template<class T>
void avoid_unused_typedef()
{}

template<class Iterator>
void check_iterator_concept(Iterator it, bool is_const)
{
    static_assert(std::is_same<std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>::value, "kek");
    assert(is_const_ref<decltype((*it))>::value == is_const);

    using a = typename std::iterator_traits<Iterator>::value_type;
    using b = typename std::iterator_traits<Iterator>::reference;
    using c = typename std::iterator_traits<Iterator>::pointer;
    using d = typename std::iterator_traits<Iterator>::difference_type;
    avoid_unused_typedef<a>();
    avoid_unused_typedef<b>();
    avoid_unused_typedef<c>();
    avoid_unused_typedef<d>();

    Iterator tmp;
    tmp = it;
    assert(tmp == it);
    assert(tmp == it++);

    Iterator tmp2;
    using std::swap;
    swap(tmp2, it);
    assert(tmp != it);
    swap(tmp2, it);

    assert(++tmp == it);
    assert(++tmp != it);


    (void)*it;
    (void)it.operator->();
}

template<class Iterator, class ConstIterator>
void check_iterator_conversion()
{
    Iterator it;
    ConstIterator cit = it;
    (void)cit;
}

//using simple_graph_t    = au::graph<int, int>;
using simple_graph_t    = au::shared_graph<int, int>;

simple_graph_t make_simple_graph()
{
    simple_graph_t g;
    g.add_vertex(1);
    g.add_vertex(2);
    g.add_vertex(3);
    g.add_vertex(4);

    auto v1 = g.find_vertex(1);
    auto v2 = g.find_vertex(2);
    auto v3 = g.find_vertex(3);
    auto v4 = g.find_vertex(4);

    g.add_edge(v1, v2, 1);
    g.add_edge(v1, v3, 2);
    g.add_edge(v2, v3, 3);
    g.add_edge(v3, v1, 1);

    g.add_edge(v4, v1, 2);
    g.add_edge(v4, v2, 2);
    g.add_edge(v4, v3, 10);

    return g;
}

void check_graph_concept()
{
    auto g = make_simple_graph();
    auto const &cg = g;

    check_iterator_concept(g.vertex_begin(), true);
    check_iterator_concept(cg.vertex_begin(), true);
    check_iterator_concept(g.edge_begin(g.find_vertex(1)), false);
    check_iterator_concept(cg.edge_begin(cg.find_vertex(1)), true);

    check_iterator_conversion<decltype(g)::vertex_iterator, decltype(g)::vertex_const_iterator>();
    check_iterator_conversion<decltype(g)::edge_iterator, decltype(g)::edge_const_iterator>();

    static_assert(std::is_same<decltype(g)::vertex_data, int>::value, "kek");
    static_assert(std::is_same<decltype(g)::edge_data, int>::value, "kek");

    decltype(g)::vertex_iterator       v1 = g.edge_begin(g.find_vertex(1)).from();
    decltype(g)::vertex_const_iterator cv1 = cg.edge_begin(cg.find_vertex(1)).from();

    decltype(g)::vertex_iterator       v2 = g.edge_begin(g.find_vertex(1)).to();
    decltype(g)::vertex_const_iterator cv2 = cg.edge_begin(cg.find_vertex(1)).to();

    assert(*v1 == 1  && *cv1 == 1);
    assert(*v2 == 2  || *v2 == 3);
    assert(*cv2 == 2 || *cv2 == 3);
}

void check_removing()
{
    auto g = make_simple_graph();

    auto v1 = g.find_vertex(1);
    auto v2 = g.find_vertex(2);
    auto v3 = g.find_vertex(3);

    g.remove_edge(g.find_edge(v2, v3));
    assert(g.edge_begin(v2) == g.edge_end(v2));

    g.remove_edge(g.find_edge(v1, v2));
    assert(g.edge_begin(v1) != g.edge_end(v1));

    g.remove_vertex(v3);
    v1 = g.find_vertex(1);
    assert(g.edge_begin(v1) == g.edge_end(v1));
}

void check_edge_iterator()
{
    const int max_id = 10000;

    auto g = make_simple_graph();
    for (int i = 4; i < max_id; ++i)
        g.add_vertex(i);

    auto e_it = g.find_edge(g.find_vertex(1), g.find_vertex(2));

    assert(*e_it.from() == 1);
    assert(*e_it.to()   == 2);
}

using simple_filtered_t = au::filtered_graph<simple_graph_t, std::function<bool(int)>, std::function<bool(int)>>;

simple_filtered_t make_simple_filtered(simple_graph_t const &g)
{
    auto vertex_predicate = [](int vertex_data)
    {
        return vertex_data != 3;
    };

    auto edge_predicate = [](int edge_data)
    {
        return edge_data != 1;
    };

    return simple_filtered_t(g, vertex_predicate, edge_predicate);
}

void check_filtered_graph()
{
    auto g  = make_simple_graph();
    auto fg = make_simple_filtered(g);

    auto v1 = fg.find_vertex(1);
    auto v2 = fg.find_vertex(2);
    auto v3 = fg.find_vertex(3);
    auto v4 = fg.find_vertex(4);

    assert(v1 != fg.vertex_end());
    assert(v2 != fg.vertex_end());
    assert(v3 == fg.vertex_end());
    assert(v4 != fg.vertex_end());

    check_iterator_concept(fg.vertex_begin(), true);
    check_iterator_concept(fg.edge_begin(v4), true);

    using std::distance;
    assert(distance(fg.edge_begin(v1), fg.edge_end(v1)) == 0);
    assert(distance(fg.edge_begin(v2), fg.edge_end(v2)) == 0);
    assert(distance(fg.edge_begin(v4), fg.edge_end(v4)) == 2);

    assert(fg.find_edge(v1, v2) == fg.edge_end(v1));
    assert(*fg.find_edge(v4, v2) == 2);
}

template<class Graph>
std::vector<int> collect_vertex_path(Graph const &g, int vertex_from, int vertex_to)
{
    auto it_from = g.find_vertex(vertex_from);
    auto it_to   = g.find_vertex(vertex_to);

    std::vector<int> res;
    auto edge_visitor = [&res](typename Graph::edge_const_iterator e_it)
    {
        res.push_back(*e_it.from());
    };

    auto len_functor = [](int value)
    {
        return static_cast<double>(value);
    };

    bool path_found = au::find_shortest_path(g, it_from, it_to, len_functor, edge_visitor);

    assert(path_found != res.empty() || it_from == it_to);
    if (path_found)
        res.push_back(*it_to);

    return res;
}

void check_shortest_path()
{
    auto g  = make_simple_graph();
    auto fg = make_simple_filtered(g);

    assert((collect_vertex_path(g, 1, 2) == std::vector<int>{1, 2}));
    assert((collect_vertex_path(g, 1, 3) == std::vector<int>{1, 3}));
    assert((collect_vertex_path(g, 1, 4) == std::vector<int>{}));
    assert((collect_vertex_path(g, 1, 1) == std::vector<int>{1}));

    assert((collect_vertex_path(g, 4, 1) == std::vector<int>{4, 1}));
    assert((collect_vertex_path(g, 4, 3) == std::vector<int>{4, 1, 3}));
    assert((collect_vertex_path(g, 4, 4) == std::vector<int>{4}));

    assert((collect_vertex_path(fg, 4, 1) == std::vector<int>{4, 1}));
    assert((collect_vertex_path(fg, 1, 2) == std::vector<int>{}));
    assert((collect_vertex_path(fg, 4, 4) == std::vector<int>{4}));
}

void final_test() {
    auto g = make_simple_graph();
    g.remove_vertex(g.vertex_end());
    g.remove_edge(g.find_edge(g.vertex_end(), g.vertex_end()));
    g.remove_edge(g.find_edge(g.find_vertex(1), g.vertex_end()));
    g.remove_edge(g.find_edge(g.vertex_end(), g.find_vertex(2)));
    g.remove_edge(g.find_edge(g.find_vertex(10), g.find_vertex(16)));
    assert(g.find_edge(g.vertex_end(), g.vertex_end()) == g.edge_end(g.vertex_end()));

    g.remove_vertex(g.find_vertex(3));
    g.remove_vertex(g.find_vertex(2));
    g.remove_vertex(g.find_vertex(1));
    g.remove_vertex(g.find_vertex(4));

    assert(g.find_vertex(1) == g.vertex_end());
    assert(g.find_vertex(2) == g.vertex_end());
    assert(g.find_vertex(3) == g.vertex_end());
    assert(g.find_vertex(4) == g.vertex_end());

    auto g2 = make_simple_graph();
    g2.remove_vertex(g2.find_vertex(2));
    assert(g2.edge_begin(g2.find_vertex(1)) != g2.edge_end(g2.find_vertex(1)));
    assert(g2.edge_begin(g2.find_vertex(2)) == g2.edge_end(g2.find_vertex(2)));
    assert(g2.edge_begin(g2.find_vertex(4)) != g2.edge_end(g2.find_vertex(4)));
    g2.remove_edge(g2.find_edge(g2.find_vertex(4), g2.find_vertex(1)));
    assert(g2.find_edge(g2.find_vertex(4), g2.find_vertex(1)) == g2.edge_end(g2.find_vertex(4)));

    g2.remove_vertex(g2.find_vertex(1));
    assert(g2.find_edge(g2.find_vertex(4), g2.find_vertex(3)) != g2.edge_end(g2.find_vertex(4)));
    assert(g2.edge_begin(g2.find_vertex(3)) == g2.edge_end(g2.find_vertex(3)));

    g2.remove_vertex(g2.find_vertex(3));
    g2.remove_vertex(g2.find_vertex(4));
    g2.remove_vertex(g2.find_vertex(1));

}

int main()
{

    check_graph_concept();
    check_removing();
    check_edge_iterator();
    check_filtered_graph();
    check_shortest_path();
    final_test();

    return 0;
}

#endif // main