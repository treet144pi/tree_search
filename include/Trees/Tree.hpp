#pragma once

#include <functional>

namespace Trees {

    template <typename KeyT, typename Comp = std::less<KeyT>>
    class SearchTree {
        private:
            struct Node
            {
                KeyT key_;
                Node *left_   = nullptr;
                Node *right_  = nullptr;
                Node *parent_ = nullptr;
                int  height_  = 1;
                int  size_    = 1;
            };

            using iterator = Node *;

            iterator top_     = nullptr; // root tree;
            Comp     cmp_; // comparator


        public: // modifiers
            void    insert(KeyT key);

        private: // Insertion helpers
            iterator bst_insert(KeyT key); // standard insert in binary search tree

        private: // Balancing
            int      node_height(iterator node) const { return node ? node->height_ : 0; }
            int      node_size(iterator node) const { return node ? node->size_ : 0; }
            void     update_metric(iterator root);

            iterator rotate_left(iterator root);
            iterator rotate_right(iterator root);

            iterator balance(iterator root, int bf);
            void     rebalance(iterator root);
            int      balance_factor(iterator current_root) const;

        public: // distance helpers

            iterator successor(iterator current) const;// the next element by value
            iterator minimum(iterator current_node) const; // to find leftmost
            int count_before(const KeyT& key) const; // count elements less than key

        public: // selectors

            iterator lower_bound(KeyT key) const; // first not less than key
            iterator upper_bound(KeyT key) const; // first greater then key
            int      distance(iterator fst,iterator snd) const;
            int      range_query(KeyT a,KeyT b) const;

        private: // memory management
            iterator clone_subtree(iterator origin, iterator parent);
            void     destroy_subtree(iterator node);

        public:
            SearchTree() = default;

            SearchTree(const SearchTree& other_tree);
            SearchTree& operator=(const SearchTree& other_tree);
            SearchTree(SearchTree && other_tree);
            SearchTree& operator=(SearchTree&& other_tree);
            ~SearchTree();

        public: // for unit test method
            iterator root() const { return top_; }

    };

//-----------------------------------------------------------------------------------------------------
//--------------------------- The Rule of Five -------------------------------------------------------
    template <typename KeyT, typename Comp>
    SearchTree<KeyT, Comp>::SearchTree(const SearchTree& other_tree): top_(nullptr), cmp_(other_tree.cmp_)
    {
        top_ = clone_subtree(other_tree.top_,nullptr);
    }
//-----------------------------------------------------------------------------------------------------
    template <typename KeyT, typename Comp>
    SearchTree<KeyT, Comp>::~SearchTree()
    {
        destroy_subtree(top_);
        top_ = nullptr;
    }

//-----------------------------------------------------------------------------------------------------
    template <typename KeyT, typename Comp>
    SearchTree<KeyT, Comp>& SearchTree<KeyT, Comp>::operator=(const SearchTree& other_tree)
    {
        if (this == &other_tree) return *this;

        iterator new_top = clone_subtree(other_tree.top_, nullptr);
        destroy_subtree(top_);

        top_   = new_top;
        cmp_   = other_tree.cmp_;

        return *this;
    }

//-----------------------------------------------------------------------------------------------------
    template <typename KeyT, typename Comp>
    SearchTree<KeyT, Comp>::SearchTree(SearchTree&& other_tree): top_(other_tree.top_), cmp_(std::move(other_tree.cmp_))
    {
        other_tree.top_ = nullptr;
    }

//-----------------------------------------------------------------------------------------------------
    template <typename KeyT, typename Comp>
    SearchTree<KeyT, Comp>& SearchTree<KeyT, Comp>::operator=(SearchTree&& other_tree)
    {
        if (this == &other_tree) return *this;

        destroy_subtree(top_);
        top_             = other_tree.top_;
        cmp_             = std::move(other_tree.cmp_);
        other_tree.top_  = nullptr;

        return *this;

    }

//-----------------------------------------------------------------------------------------------------
//--------------------------- Memory management -------------------------------------------------------

    template <typename KeyT, typename Comp >
    typename SearchTree<KeyT, Comp>::iterator
    SearchTree<KeyT, Comp>::clone_subtree(iterator origin,iterator parent)
    {
        if (!origin)   return nullptr;

        iterator node = new Node{ origin->key_};
        node->parent_ = parent;
        node->height_ = origin->height_;
        node->size_   = origin->size_;

        node->left_   = clone_subtree(origin->left_, node);
        node->right_  = clone_subtree(origin->right_, node);

        return node;

    }

//-----------------------------------------------------------------------------------------------------

    template<typename KeyT, typename Comp>
    void SearchTree<KeyT, Comp>::destroy_subtree(iterator node)
    {
        if (!node) return;

        destroy_subtree(node->left_);
        destroy_subtree(node->right_);

        delete node;
    }
//-----------------------------------------------------------------------------------------------------
//--------------------------- Distance helpers  -------------------------------------------------------

    template <typename KeyT, typename Comp >
    typename SearchTree<KeyT, Comp>::iterator
    SearchTree<KeyT, Comp>::minimum(iterator current_node) const
    {
        if (!current_node) return nullptr;

        while (current_node->left_)
            current_node = current_node->left_;
        return current_node;
    }
//-----------------------------------------------------------------------------------------------------
    template <typename KeyT, typename Comp >
    typename SearchTree<KeyT, Comp>::iterator
    SearchTree<KeyT, Comp>::successor(iterator current) const
    {
        if (!current) return nullptr;

        if (current->right_)
            return minimum(current->right_);

        iterator parent = current->parent_;
        while (parent && parent->right_ == current)
        {
            current= current->parent_;
            parent = current->parent_;
        }

        return parent;
    }
    template <typename KeyT, typename Comp >
    int SearchTree<KeyT,Comp>::count_before(const KeyT& key) const
    {
        int counter         = 0;
        iterator cur_it     = top_;

        while (cur_it)
        {
            if (cmp_(key,cur_it->key_)) cur_it = cur_it->left_;
            else if (cmp_(cur_it->key_, key))
            {
                counter += 1 + node_size(cur_it->left_);
                cur_it   = cur_it->right_;
            }
            else
            {
                counter += node_size(cur_it->left_);
                break;
            }

        }


            return counter;

    }
//-----------------------------------------------------------------------------------------------------
//--------------------------- Selectors ---------------------------------------------------------------

    template <typename KeyT, typename Comp >
    int SearchTree<KeyT, Comp>::range_query(KeyT a, KeyT b) const
    {
        if (!cmp_(a,b)) return 0;

        iterator fst = lower_bound(a);
        iterator snd = upper_bound(b);
        return distance(fst,snd);
    }

//-----------------------------------------------------------------------------------------------------
    template <typename KeyT, typename Comp >
    int SearchTree<KeyT, Comp>::distance(iterator fst,iterator snd) const
    {

        if (fst == nullptr) return 0;
        if (fst == snd) return 0;

        int count_fst =  fst ? count_before(fst->key_) : 0;
        int count_snd =  snd ? count_before(snd->key_) : node_size(top_);

        return (count_snd - count_fst);
    }
//------------------------------------------------------------------------------------------------------
    template <typename KeyT, typename Comp >
    typename SearchTree<KeyT, Comp>::iterator
    SearchTree<KeyT, Comp>::lower_bound(KeyT key) const
    {
        iterator current_node = top_;
        iterator best_node    = nullptr;
        while (current_node)
        {
            if (!cmp_(current_node->key_,key))
            {
                best_node = current_node;
                current_node = current_node->left_;
            }
            else
                current_node = current_node->right_;
        }

        return best_node;

    }
//------------------------------------------------------------------------------------------------------
    template <typename KeyT, typename Comp >
    typename SearchTree<KeyT, Comp>::iterator
    SearchTree<KeyT, Comp>::upper_bound(KeyT key) const
    {
        iterator current_node = top_;
        iterator best_node    = nullptr;
        while (current_node)
        {
            if (cmp_(key, current_node->key_))
            {
                best_node = current_node;
                current_node = current_node->left_;
            }
            else
                current_node = current_node->right_;
        }

        return best_node;

    }

//------------------------------------------------------------------------------------------------------
//----------------------------- Balancing --------------------------------------------------------------
    template <typename KeyT, typename Comp>
    void SearchTree<KeyT, Comp>::update_metric(iterator root)
    {

        int max_height = node_height(root->left_) > node_height(root->right_)  ? node_height(root->left_): node_height(root->right_);
        root->height_ =  1 + max_height;
        root->size_   =  1 + node_size(root->left_) + node_size(root->right_);

    }


//-------------------------------------------------------------------------------------------------------------
    template <typename KeyT, typename Comp>
    int SearchTree<KeyT,Comp>::balance_factor(iterator current_root) const
    {
        int bf = 0;
        if (current_root->right_)
            bf -= current_root->right_->height_;

        if (current_root->left_)
            bf += current_root->left_->height_;
        return bf;
    }
//-------------------------------------------------------------------------------------------------------------


    template <typename KeyT,typename Comp>
    void SearchTree<KeyT, Comp>::rebalance(iterator root)
    {
        iterator current_root = root->parent_;
        while (current_root)
        {

            update_metric(current_root);
            int bf  = balance_factor(current_root);

            if (bf <-1 ||bf > 1)
            {
                current_root = balance(current_root,bf);
            }
            current_root = current_root->parent_;
        }
    }
//-------------------------------------------------------------------------------------------------------------

    template <typename KeyT, typename Comp >
    typename SearchTree<KeyT, Comp>::iterator
    SearchTree< KeyT, Comp>::rotate_right(iterator root)
    {


        iterator new_root    = root->left_;

        new_root ->parent_   = root->parent_;

        if (root->parent_)
        {
            if (root->parent_->left_ == root)
                root->parent_->left_ = new_root;
            else
                root->parent_->right_ = new_root;

        }
        else
        {
            top_ = new_root;

        }
        iterator temp_right  = new_root->right_;

        new_root->right_     = root;
        root->left_          = temp_right;

        root->parent_            = new_root;

        if (temp_right) temp_right->parent_ = root;
        update_metric(root);
        update_metric(new_root);

        return new_root;
    }
//-------------------------------------------------------------------------------------------------------------


    template <typename KeyT, typename Comp >
    typename SearchTree<KeyT, Comp>::iterator
    SearchTree< KeyT, Comp>::rotate_left(iterator root)
    {

        iterator new_root    = root->right_;

        new_root ->parent_   = root->parent_;

        if (root->parent_)
        {
            if (root->parent_->left_ == root)
                root->parent_->left_ = new_root;
            else
                root->parent_->right_ = new_root;

        }
        else
        {
            top_ = new_root;

        }
        iterator temp_left  = new_root->left_;

        new_root->left_    = root;
        root->right_       = temp_left;

        root->parent_      = new_root;

        if (temp_left) temp_left->parent_ = root;
        update_metric(root);
        update_metric(new_root);

        return new_root;
    }
//-------------------------------------------------------------------------------------------------------------


    template <typename KeyT, typename Comp >
    typename SearchTree<KeyT, Comp>::iterator
    SearchTree< KeyT, Comp>::balance(iterator root, int bf)
    {
        if (bf > 1)
        {
            if (root->left_ && balance_factor(root->left_) < 0)
                root->left_ = rotate_left(root->left_);
            root = rotate_right(root);
        }
        else if (bf < -1)
        {
            if (root->right_ && balance_factor(root ->right_) > 0)
                root->right_ = rotate_right(root->right_);
            root = rotate_left(root);
        }

        return root;
    }
//-------------------------------------------------------------------------------------------------------------
//---------------------- Insertion helpers --------------------------------------------------------
    template <typename KeyT, typename Comp >
    typename SearchTree<KeyT, Comp>::iterator
    SearchTree< KeyT, Comp>::bst_insert(KeyT key)
    {
        if (!top_) // top_ == nullptr
        {
            top_ = new Node{key};
            return top_;
        }

        iterator parent = nullptr;
        iterator child  = top_;

        while (child)
        {
            parent = child;

            if (cmp_(child->key_,key))
                child = child->right_; // go right
            else if (cmp_(key,child->key_))
                child = child->left_; // go left
            else
                return child; // not duplicate
        }

        child          = new Node{key};

        child->parent_ = parent;

        if (cmp_(parent->key_,key))
            parent->right_ = child;
        else
            parent->left_  = child;

        return child;
    }

//-------------------------------------------------------------------------------------------------------
//---------------------------- modifiers ----------------------------------------------------------------

    template <typename KeyT, typename Comp >
    void SearchTree< KeyT, Comp>::insert(KeyT key)
    {
        iterator new_node = bst_insert(key);
        rebalance(new_node);
    }
//--------------------------------------------------------------------------------------------------------


}
