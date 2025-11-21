#include "runner_set.hpp"
#include <set>
#include <chrono>
#include <iterator>
#include <iostream>
#include <stdexcept>

int launcher_set(std::istream& in, std::ostream& out, bool benchmark)
{
    using clock = std::chrono::steady_clock;
    using ns    = std::chrono::nanoseconds;

    std::set<int> tree;
    char op;
    ns acc{0};
    try
    {
        while (in >> op)
        {
            if (op == 'k')
            {
                int x;
                in >> x;
                if (!in.good())
                {
                    throw std::runtime_error("failed to read ");
                }
                if (benchmark)
                {
                    auto t0 = clock::now();
                    tree.insert(x);
                    auto t1 = clock::now();
                    acc += (t1 - t0);
                } else
                {
                    tree.insert(x);
                }
            }
            else if (op == 'q')
            {
                int a, b;
                in >> a >> b;
                if (!in.good())
                {
                    throw std::runtime_error("failed to read ");
                }
                if (benchmark) {
                    auto t0 = clock::now();

                    int ans = 0;
                    if (b > a) {
                        auto fst = tree.lower_bound(a);
                        auto snd = tree.upper_bound(b);
                        ans = std::distance(fst, snd);
                    }

                    auto t1 = clock::now();
                    acc += (t1 - t0);
                } else {
                    int ans = 0;
                    if (b > a) {
                        auto fst = tree.lower_bound(a);
                        auto snd = tree.upper_bound(b);
                        ans = std::distance(fst, snd);
                    }
                    out << ans << ' ';
                }
            }
        }
    }
    catch (const std::exception& ex)
    {
        out << ex.what() << '\n';
        return 1;
    }

    if (benchmark)
    {
        auto nas = std::chrono::duration_cast<std::chrono::milliseconds>(acc).count();
        out << nas << " ms\n";
    }
    else
    {
        out << '\n';
    }
    return 0;
}
