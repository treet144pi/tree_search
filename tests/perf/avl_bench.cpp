#include <benchmark/benchmark.h>
#include <random>
#include <vector>
#include <Trees/Tree.hpp>

static std::vector<int> make_data(size_t n, uint32_t seed=52)
{
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> d(-2000000, 2000000);
    std::vector<int> v(n); for (auto& x: v) x = d(rng); return v;
}

//---------------------------------------------------------------------------------------------------------------

static void Insert(benchmark::State& st) {
    auto data = make_data(st.range(0));


    for (auto _ : st)
    {
        Trees::SearchTree<int> t;
        for (int i : data) t.insert(i);
        benchmark::DoNotOptimize(t.root());
    }
}
BENCHMARK(Insert)->Arg(1000)->Arg(10000)->Arg(100000);


//---------------------------------------------------------------------------------------------------------------
static void LowerBound(benchmark::State& st) {
    Trees::SearchTree<int> t;

    auto data = make_data(st.range(0));
    for (int i: data) t.insert(i);
    auto queries = make_data(10000, 500);

    for (auto _ : st)
    {
        size_t a=0;
        for (int q : queries) { if (t.lower_bound(q)) ++a; }
        benchmark::DoNotOptimize(a);
    }
}
BENCHMARK(LowerBound)->Arg(10000)->Arg(100000);
//---------------------------------------------------------------------------------------------------------------
static void UpperBound(benchmark::State& st)
{
    Trees::SearchTree<int> t;

    auto data = make_data(st.range(0));
    for (int i: data) t.insert(i);


    auto queries = make_data(10000,500);

    for (auto _ : st)
    {
        size_t a=0;
        for (int q : queries) { if (t.lower_bound(q)) ++a; }
        benchmark::DoNotOptimize(a);
    }
}
BENCHMARK(UpperBound) ->Arg(10000)->Arg(100000);

//---------------------------------------------------------------------------------------------------------------
static void RangeQuery(benchmark::State& st) {
    Trees::SearchTree<int> t;
    auto data = make_data(st.range(0));
    for (int i: data) t.insert(i);


    std::mt19937 rng(123);
    std::uniform_int_distribution<int> d(-1000000, 1000000);
    std::vector<std::pair<int,int>> qs(10000);


    for (auto& q: qs){ int a=d(rng), b=d(rng); if(a>b) std::swap(a,b); q={a,b}; }

    for (auto _ : st)
    {
        long long s=0;
        for (auto [a,b]: qs) s += t.range_query(a,b);
        benchmark::DoNotOptimize(s);
    }
}
BENCHMARK(RangeQuery)->Arg(10000)->Arg(100000);


BENCHMARK_MAIN();
