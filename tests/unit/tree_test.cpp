#include <gtest/gtest.h>
#include <Trees/Tree.hpp>
#include <random>
#include <vector>
using ST = Trees::SearchTree<int>;
static std::vector<int> make_data(size_t n, uint32_t seed=42) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> d(-1'000'000, 1'000'000);
    std::vector<int> v(n);
    for (auto& x : v) x = d(rng);
    return v;
}

TEST(Bounds, EmptyTree)
{
    ST t;
    auto lb = t.lower_bound(89); EXPECT_EQ(lb,nullptr);
    auto ub = t.upper_bound(52); EXPECT_EQ(ub,nullptr);
}
TEST(RangeQuery, EmptyTree)
{
    ST t;
    EXPECT_EQ(t.range_query(40,50),0);
}


TEST(Bounds, LowerUpper) {
    ST t; for (int x : {10,20,30,40}) t.insert(x);

    auto lb20 = t.lower_bound(20); ASSERT_NE(lb20, nullptr); EXPECT_EQ(lb20->key_, 20);
    auto ub20 = t.upper_bound(20); ASSERT_NE(ub20, nullptr); EXPECT_EQ(ub20->key_, 30);

    auto lb5  = t.lower_bound(5);  ASSERT_NE(lb5,  nullptr); EXPECT_EQ(lb5->key_, 10);
    auto lb41 = t.lower_bound(41); EXPECT_EQ(lb41, nullptr);

    auto ub40 = t.upper_bound(40); EXPECT_EQ(ub40, nullptr);
}

TEST(RangeQuery, HalfOpenLeftInclusiveRightOpen) {
    ST t; for (int x : {10,20,30,40}) t.insert(x);
    EXPECT_EQ(t.range_query(20,40), 3);
    EXPECT_EQ(t.range_query(5,10),  1);
    EXPECT_EQ(t.range_query(40,100),1);
    EXPECT_EQ(t.range_query(30,30), 0);
}

TEST(Inorder, SortedStrictlyIncreasing)
{
    ST t; for (int x: {5,3,7,1,4,6,8}) t.insert(x);
    std::vector<int> v;
    for (auto p = t.minimum(t.root()); p; p = t.successor(p)) v.push_back(p->key_);
    for (size_t i = 1; i < v.size(); ++i) EXPECT_LT(v[i-1], v[i]);
    EXPECT_EQ((std::vector<int>{1,3,4,5,6,7,8}), v);
}

TEST(RuleOfFive, CopyAndMove) {
    ST a; for (int x : {10,20,30}) a.insert(x);

    // copy ctor
    ST b = a;
    EXPECT_EQ(b.range_query(0,100), 3);
    EXPECT_EQ(a.range_query(0,100), 3);

    // copy assign
    ST c;
    c = b;
    EXPECT_EQ(c.range_query(0,100), 3);

    // move ctor
    ST d = std::move(a);
    EXPECT_EQ(d.range_query(0,100), 3);
    EXPECT_EQ(a.root(), nullptr);

    // move assign
    ST e;
    e = std::move(b);
    EXPECT_EQ(e.range_query(0,100), 3);
    EXPECT_EQ(b.root(), nullptr);
}

TEST(DistanceFast, MatchesStdSetOnRandomData) {
    ST t;
    std::set<int> s;

    auto data = make_data(5000);
    for (int x : data) { t.insert(x); s.insert(x); }
    std::vector<std::pair<int,int>> qs = {
        {-1'000'000, 1'000'000}, {-52,52}, {0,0}, {10,100}, {-100, -10}, {123, 456}
    };

    for (auto [a,b] : qs) {
        auto it_t_f = t.lower_bound(a);
        auto it_t_s = t.upper_bound(b);
        int got = t.distance(it_t_f, it_t_s);

        auto it_s_f = s.lower_bound(a);
        auto it_s_s = s.upper_bound(b);
        int exp = static_cast<int>(std::distance(it_s_f, it_s_s));
        EXPECT_EQ(got, exp);
    }
}
TEST(DistanceFast, EdgeCases) {
    ST t;
    std::set<int> s;
    for (int x : {10,20,30,40}) { t.insert(x); s.insert(x); }

    auto it_t_f = t.lower_bound(15);
    auto it_t_s = t.upper_bound(std::numeric_limits<int>::max());
    int got = t.distance(it_t_f, it_t_s);

    auto it_s_f = s.lower_bound(15);
    auto it_s_s = s.end();
    int exp             = static_cast<int>(std::distance(it_s_f, it_s_s));
    EXPECT_EQ(got, exp);

    ST empty;
    auto f2     = empty.lower_bound(0);
    auto s2     = empty.upper_bound(100);
    int empty_got  = empty.distance(f2,s2);
    EXPECT_EQ(empty_got, 0);

    auto f3 = t.lower_bound(40);
    auto s3 = t.upper_bound(10);
    int t_got   = t.distance(f3,s3);
    EXPECT_EQ(t_got, -2);
}
