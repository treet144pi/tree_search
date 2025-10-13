#include <gtest/gtest.h>
#include <Trees/Tree.hpp>

using ST = Trees::SearchTree<int>;

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

TEST(MinimumAndSuccessor,EmptyTree)
{
    ST t;
    EXPECT_EQ(t.minimum(t.root()), nullptr);
    EXPECT_EQ(t.successor(nullptr), nullptr);
}

TEST(Minimum,FindMiniumSubTree)
{
    ST t;
    for (int x: {20,53,89,37}) t.insert(x);
    auto min20 =t.minimum(t.root()); ASSERT_NE(min20,nullptr); EXPECT_EQ(min20->key_,20);// minimum == 20

    t.insert(5);
    auto min5 =t.minimum(t.root()); ASSERT_NE(min5,nullptr); EXPECT_EQ(min5->key_,5);// minimum == 5

    t.insert(6);
    auto remin5 =t.minimum(t.root()); ASSERT_NE(remin5,nullptr); EXPECT_EQ(remin5->key_,5); // minimum == 5

    t.insert(1);
    auto min1 =t.minimum(t.root()); ASSERT_NE(min1,nullptr); EXPECT_EQ(min1->key_,1);// minimum == 1

    t.insert(-6);
    auto minminus6 =t.minimum(t.root()); ASSERT_NE(minminus6,nullptr); EXPECT_EQ(minminus6->key_,-6);// minimum == 1


}

TEST(InsertAndOrder, BasicInorderBySuccessor) {
    ST t;
    for (int x : {30, 10, 40, 20}) t.insert(x);

    auto cur = t.lower_bound(std::numeric_limits<int>::min());
    if (!cur) cur = t.root();
    std::vector<int> got;
    auto min = t.root();
    while (min && min->left_) min = min->left_;
    for (auto p = min; p; p = t.successor(p)) got.push_back(p->key_);

    EXPECT_EQ((std::vector<int>{10,20,30,40}), got);
}

TEST(Bounds, LowerUpper) {
    ST t; for (int x : {10,20,30,40}) t.insert(x);

    auto lb20 = t.lower_bound(20); ASSERT_NE(lb20, nullptr); EXPECT_EQ(lb20->key_, 20);
    auto ub20 = t.upper_bound(20); ASSERT_NE(ub20, nullptr); EXPECT_EQ(ub20->key_, 30);

    auto lb5  = t.lower_bound(5);  ASSERT_NE(lb5,  nullptr); EXPECT_EQ(lb5->key_, 10);
    auto lb41 = t.lower_bound(41); EXPECT_EQ(lb41, nullptr);

    auto ub40 = t.upper_bound(40); EXPECT_EQ(ub40, nullptr);
}

TEST(RangeQuery, HalfOpenRightInclusive) {
    ST t; for (int x : {10,20,30,40}) t.insert(x);
    EXPECT_EQ(t.range_query(20,40), 2); // (20,40] -> {30,40}
    EXPECT_EQ(t.range_query(5,10),  1); // (5,10]  -> {10}
    EXPECT_EQ(t.range_query(40,100),0); // (40,100] -> {}
    EXPECT_EQ(t.range_query(30,30), 0); // a>=b -> 0
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
