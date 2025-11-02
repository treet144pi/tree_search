#include "runner_set.hpp"
#include <set>
#include <chrono>
#include <iterator>
#include <iostream>
int launcher_set(std::istream& in, std::ostream& out, bool benchmark)
{
    using clock = std::chrono::steady_clock;
    using ns    = std::chrono::nanoseconds;

    std::set<int> tree;
    char op;
    ns acc{0};
    while (in >> op)
    {
        if (op == 'k')
        {
            int x;
            in >> x;
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

    if (benchmark)
    {
        auto nas = std::chrono::duration_cast<std::chrono::nanoseconds>(acc).count();
        out << nas << " ns\n";
    }
    else
    {
        out << '\n';
    }
    return 0;
}
