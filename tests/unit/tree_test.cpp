#include <gtest/gtest.h>
#include <Trees/Tree.hpp>

using ST = Trees::SearchTree<int>;


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
