#pragma once

#include <functional>
#include <vector>
#include <stdexcept>

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

            struct Block_Memory
            {
                iterator begin_ = nullptr;
                iterator cur_   = nullptr;
                iterator end_   = nullptr; // after past
            };

            std::vector<Block_Memory> mem_blocks_;

        public: // modifiers
            void    insert(const KeyT& key);

        private: // Insertion helpers
            iterator bst_insert(const KeyT& key); // standard insert in binary search tree

        private: // Balancing
            int            node_height(iterator node) const { return node ? node->height_ : 0; }
            int            node_size(iterator node) const { return node ? node->size_ : 0; }
            inline void    update_metric(iterator root);

            iterator rotate_left(iterator root);
            iterator rotate_right(iterator root);

            iterator balance(iterator root, int bf);
            void     rebalance(iterator root);
            int      balance_factor(iterator current_root) const;

        private: // distance helpers

            int      count_before(const KeyT& key) const; // count elements less than key

        public: // selectors

            iterator lower_bound(const KeyT& key) const; // first not less than key
            iterator upper_bound(const KeyT& key) const; // first greater then key
            int      distance(iterator fst,iterator snd) const;
            int      range_query(const KeyT& a,const KeyT& b) const;

        private: // memory management
            void     add_block();
            iterator get_node(const KeyT& key);
            void     destroy_blocks_memory();

            iterator clone_subtree(iterator origin, iterator parent);

        public:
            SearchTree() = default;

            SearchTree(const SearchTree& other_tree);
            SearchTree& operator=(const SearchTree& other_tree);
            SearchTree(SearchTree && other_tree);
            SearchTree& operator=(SearchTree&& other_tree);
            virtual ~SearchTree();

        public: // for unit test method
            iterator root() const { return top_; }

    };

//-----------------------------------------------------------------------------------------------------
//--------------------------- The Rule of Five -------------------------------------------------------
    template <typename KeyT, typename Comp>
    SearchTree<KeyT, Comp>::SearchTree(const SearchTree& other_tree): top_(nullptr), cmp_(other_tree.cmp_)
    {
        try {
            top_ = clone_subtree(other_tree.top_,nullptr);
        }
        catch (...) {
            destroy_blocks_memory();
            throw;
        }
    }
//-----------------------------------------------------------------------------------------------------
    template <typename KeyT, typename Comp>
    SearchTree<KeyT, Comp>::~SearchTree()
    {
        destroy_blocks_memory();
        top_ = nullptr;
    }

//-----------------------------------------------------------------------------------------------------
    template <typename KeyT, typename Comp>
    SearchTree<KeyT, Comp>& SearchTree<KeyT, Comp>::operator=(const SearchTree& other_tree)
    {
        if (this == &other_tree) return *this;

        SearchTree tmp{other_tree};

        std::swap(top_,       tmp.top_);
        std::swap(cmp_,       tmp.cmp_);
        std::swap(mem_blocks_,tmp.mem_blocks_);

        return *this;
    }

//-----------------------------------------------------------------------------------------------------
    template <typename KeyT, typename Comp>
    SearchTree<KeyT, Comp>::SearchTree(SearchTree&& other_tree): top_(other_tree.top_), cmp_(std::move(other_tree.cmp_)),
                                                                 mem_blocks_(std::move(other_tree.mem_blocks_))
    {
        other_tree.top_ = nullptr;
    }

//-----------------------------------------------------------------------------------------------------
    template <typename KeyT, typename Comp>
    SearchTree<KeyT, Comp>& SearchTree<KeyT, Comp>::operator=(SearchTree&& other_tree)
    {
        if (this == &other_tree) return *this;

        destroy_blocks_memory();
        top_             = other_tree.top_;
        cmp_             = std::move(other_tree.cmp_);
        mem_blocks_      = std::move(other_tree.mem_blocks_);

        other_tree.top_  = nullptr;

        return *this;

    }

//-----------------------------------------------------------------------------------------------------
//--------------------------- Memory management -------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
    template <typename KeyT, typename Comp>
    void SearchTree<KeyT, Comp>::add_block()
    {
        size_t prev_capacity = mem_blocks_.empty() ? 0: static_cast<size_t> (mem_blocks_.back().end_ - mem_blocks_.back().begin_);
        size_t new_capacity  = prev_capacity ? prev_capacity*2: 512;

        iterator new_mem   = static_cast<iterator> (::operator new[](new_capacity * sizeof(Node)));
        mem_blocks_.push_back(Block_Memory{new_mem,new_mem,new_mem+new_capacity});
    }

    template <typename KeyT, typename Comp >
    typename SearchTree<KeyT, Comp>::iterator
    SearchTree<KeyT, Comp>::get_node(const KeyT& key)
    {
        if (mem_blocks_.empty() || (mem_blocks_.back().cur_ == mem_blocks_.back().end_)) add_block();

        Block_Memory& last_block = mem_blocks_.back();
        iterator cur_node        = last_block.cur_;
        ::new (cur_node) Node{key};
        last_block.cur_++; // after memory allocation
        return cur_node;
    }


    template <typename KeyT, typename Comp>
    void SearchTree<KeyT, Comp>::destroy_blocks_memory()
    {
        for (auto& m_b :mem_blocks_)
        {
            for (iterator it = m_b.begin_; it != m_b.cur_;++it)
                it->~Node();
            ::operator delete[](m_b.begin_);
        }
        mem_blocks_.clear();
    }

    template <typename KeyT, typename Comp >
    typename SearchTree<KeyT, Comp>::iterator
    SearchTree<KeyT, Comp>::clone_subtree(iterator origin,iterator parent)
    {
        if (!origin)   return nullptr;

        iterator node = get_node(origin->key_);
        node->parent_ = parent;
        node->height_ = origin->height_;
        node->size_   = origin->size_;

        node->left_   = clone_subtree(origin->left_, node);
        node->right_  = clone_subtree(origin->right_, node);

        return node;

    }


//-----------------------------------------------------------------------------------------------------
//--------------------------- Distance helpers  -------------------------------------------------------
//-----------------------------------------------------------------------------------------------------

    template <typename KeyT, typename Comp >
    int SearchTree<KeyT,Comp>::count_before(const KeyT& key) const
    {
        int counter         = 0;
        iterator cur_it     = top_;

        while (cur_it)
        {
            const KeyT& k = cur_it->key_;
            if (cmp_(key,k)) cur_it = cur_it->left_;
            else if (cmp_(k, key))
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
//-----------------------------------------------------------------------------------------------------

    template <typename KeyT, typename Comp >
    int SearchTree<KeyT, Comp>::range_query(const KeyT& a, const KeyT& b) const
    {
        if (!cmp_(a,b))
        {
           ;return 0;
        }

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

        int count_fst =  count_before(fst->key_);
        int count_snd =  snd ? count_before(snd->key_) : node_size(top_);
        return (count_snd - count_fst);
    }
//------------------------------------------------------------------------------------------------------
    template <typename KeyT, typename Comp >
    typename SearchTree<KeyT, Comp>::iterator
    SearchTree<KeyT, Comp>::lower_bound(const KeyT& key) const
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
    SearchTree<KeyT, Comp>::upper_bound(const KeyT& key) const
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
//-----------------------------------------------------------------------------------------------------
    template <typename KeyT, typename Comp>
    inline void SearchTree<KeyT, Comp>::update_metric(iterator root)
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
//-----------------------------------------------------------------------------------------------------
//---------------------- Insertion helpers ------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
    template <typename KeyT, typename Comp >
    typename SearchTree<KeyT, Comp>::iterator
    SearchTree< KeyT, Comp>::bst_insert(const KeyT& key)
    {
        if (!top_) // top_ == nullptr
        {
            top_ = get_node(key);
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

        child          = get_node(key);

        child->parent_ = parent;

        if (cmp_(parent->key_,key))
            parent->right_ = child;
        else
            parent->left_  = child;

        return child;
    }

//-------------------------------------------------------------------------------------------------------
//---------------------------- modifiers ----------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------

    template <typename KeyT, typename Comp >
    void SearchTree< KeyT, Comp>::insert(const KeyT& key)
    {
        iterator new_node = bst_insert(key);
        rebalance(new_node);
    }
//--------------------------------------------------------------------------------------------------------


}
