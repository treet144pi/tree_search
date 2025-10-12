#include <iostream>
#include "Tree.hpp"
#include <utility>

int main()
{
    Trees::SearchTree<int> tree;
    char ch;
    std::cin >> ch;
    while (ch != EOF)
    {
        switch (ch)
        {
            case 'k':
            {
                int key;
                std::cin >> key;
                tree.insert(key);
                break;
            }
            case 'q':
            {
                int fst_key, snd_key;
                std::cin >> fst_key >> snd_key;
                std::cout << tree.range_query(fst_key,snd_key)<<' ';
                break;
            }
            default:
                break;
        }
        std::cin >> ch;
    }
    std::cout<< '\n';
    return 0;



}
