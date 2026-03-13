#include<iostream>
#include<limits>
#include<concepts>

template<typename T>
concept HasLessThan = requires(T a, T b) {
    { a < b } -> std::convertible_to<bool>;
};

template<HasLessThan T>
class AvlTree {
private:
    struct node {
        T val;
        node* left;
        node* right;
        int h;
        node(T el) : val(el), left(nullptr), right(nullptr), h(1) {}
    };
    using nodeptr = node*;
    int height(nodeptr nd) {
        return (nd ? nd->h : 0);
    }
    int bal(nodeptr nd) {
        if (!nd) return 0;
        return height(nd->left) - height(nd->right);
    }
    int setheight(nodeptr nd) {
        if (!nd) return 0;
        return nd->h = 1 + std::max(height(nd->left), height(nd->right));
    }
    nodeptr min(nodeptr nd) const {
        if (!nd || !nd->left) return nd;
        return min(nd->left);
    }
    nodeptr max(nodeptr nd) const {
        if (!nd || !nd->right) return nd;
        return max(nd->right);
    }
    nodeptr root;
    size_t sz;
    nodeptr rotate_right(nodeptr nd) {
        if (!nd || !nd->left) return nd;
        auto new_root = nd->left;
        nd->left = new_root->right;
        new_root->right = nd;
        setheight(nd);
        setheight(new_root);
        return new_root;
    }
    nodeptr rotate_left(nodeptr nd) {
        if (!nd || !nd->right) return nd;
        auto new_root = nd->right;
        nd->right = new_root->left;
        new_root->left = nd;
        setheight(nd);
        setheight(new_root);
        return new_root;
    }
    nodeptr balance(nodeptr nd) {
        setheight(nd);
        if (bal(nd) == 2) {
            if (bal(nd->right) < 0) nd->right = rotate_right(nd->right);
            return rotate_left(nd);
        }
        if (bal(nd) == -2) {
            if (bal(nd->left) > 0) nd->left = rotate_left(nd->left); 
            return rotate_right(nd);
        }
        return nd;
    }
    nodeptr add(nodeptr nd, const T& val) {
        if (!nd) {
            ++sz;
            return new node(val);
        }
        if (val < nd->val) nd->left = add(nd->left, val);
        if (val > nd->val) nd->right = add(nd->right, val);
        return balance(nd);
    }
    nodeptr delmin(nodeptr nd) {
        if (!nd) return nd;
        if (!nd->left) return nd->right;
        nd->left = delmin(nd->left);
        return balance(nd);
    }
    nodeptr del(node* nd, const T& val) {
        if (!nd) return nd;
        if (val < nd->val) {
            nd->left = del(nd->left, val);
        } else if (val > nd->val) {
            nd->right = del(nd->right, val);
        } else {
            auto left = nd->left;
            auto right = nd->right;
            delete nd;
            if (!right) return left;
            auto mn = min(right);
            mn->right = delmin(right);
            mn->left = left;
            return balance(mn);
        }
        return balance(nd);
    }
    bool contains(nodeptr nd, const T& val) const {
        if (!nd) return false;
        if (val == nd->val) return true;
        return (val < nd->val ? contains(nd->left, val) : contains(nd->right, val));
    }
public:
    AvlTree() : root(nullptr), sz(0) {}
    T min() const {
        auto ret = min(root);
        return ret->val;
    }
    T max() const {
        auto ret = max(root);
        return ret->val;
    }
    void add(const T& val) {root = add(root, val);}
    void del(const T& val) {root = del(root, val);}
    bool contains(const T& val) const {return contains(root, val);}
    AvlTree(const std::initializer_list<T>& list) {
        for (const auto& elem : list) {
            add(elem);
        }
    }
    size_t size() const {
        return sz;
    }
};

int main() {
    AvlTree<int> tree;
    for (auto i : {1, 2, 3, 4, 5}) tree.add(i);
    std::cout << tree.contains(3) << ' ' << tree.contains(85) << '\n';
    std::cout << tree.min() << ' ' << tree.max() << '\n';
    tree.add(-1), tree.add(85), tree.del(5);
    std::cout << tree.min() << ' ' << tree.max() << '\n';
    std::cout << tree.contains(5) << '\n';
    return 0;
}