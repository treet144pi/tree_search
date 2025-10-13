#include "runner.hpp"
#include <Trees/Tree.hpp>

int launcher(std::istream& in, std::ostream& out)
{

    Trees::SearchTree<int> tree;
    char op;
    while (in >> op)
    {
        if (op == 'k')
        {
            int a;
            in>>a;
            tree.insert(a);
        }
        else if (op == 'q')
        {
            int a,b;
            in >> a >> b;
            out << tree.range_query(a,b) << ' ';
        }
    }

    return 0;
}
