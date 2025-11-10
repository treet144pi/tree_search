#include "runner.hpp"
#include <Trees/Tree.hpp>
#include <chrono>
#include <iostream>
int launcher(std::istream& in, std::ostream& out, bool benchmark)
{
    using clock = std::chrono::steady_clock;
    using ns    = std::chrono::nanoseconds;

    Trees::SearchTree<int> tree;
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
            }
            else
            {
                tree.insert(x);
            }
        }
        else if (op == 'q')
        {
            int a, b;
            in >> a >> b;
            if (benchmark)
            {
                auto t0 = clock::now();

                int ans = 0;
                if (b > a)
                    ans = tree.range_query(a, b);
                auto t1 = clock::now();
                acc += (t1 - t0);
            }
            else
            {
                int ans = 0;
                if (b > a)
                    ans = tree.range_query(a, b);

                out << ans << ' ';
            }
        }
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
