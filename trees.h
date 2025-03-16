#pragma once
#include <iostream>
using namespace std;
#include <sstream>


/* Реализовано шаблонное бинарное дерево, унаследован класс АвлДерево с АвлЗвеном, но при создании обЪекта авоДерево возникают ошибки с приведением типов, пока в процессе решения проблемы*/ 


template <typename TKey, typename TVal>
struct Node {
    Node* left;
    Node* right;
    Node* parent;
    pair <TKey,TVal> data;
    Node(Node< TKey, TVal >* l = nullptr, Node< TKey, TVal >* r = nullptr, Node<TKey, TVal>* p = nullptr, pair <TKey, TVal> d = {}) {
        data = d; left = l; right = r; parent = p;
    }
    Node(const Node<TKey, TVal>& node2) {
        data = node2.data;
        left = node2.left;
        right = node2.right;
        parent = node2.parent;
    }
    Node<TKey, TVal>& operator=(const Node<TKey, TVal>& node2) {
        if (this != &node2) {
            data = node2.data;
            left = node2.left;
            right = node2.right;
            parent = node2.parent;
        }
        return *this;
    }
    bool operator==(const Node<TKey, TVal>& node2) const {
        return (data == node2.data);
    }
    bool operator!=(const Node<TKey, TVal>& node2) const {
        return (data != node2.data);
    } 
};


template <typename TKey, typename TVal>
struct avlNode : public Node<TKey,TVal> {
    unsigned char height;

};

template <typename TKey, typename TVal, typename NodeType = Node<TKey, TVal>>
class Tree {
public:
    NodeType* root;
    Tree() : root(nullptr) {}
    Tree(const TKey& k) { root = new NodeType(); root->data.first = k; };
    ~Tree()
    {
        Delete_tree(root);
    }
    NodeType* Search(const TKey& k)
    {
        if (isEmpty()) {
            throw ("Tree is empty");
        }
        // ищем звено
        NodeType* curr = root;
        NodeType* par = nullptr;
        while (curr && curr->data.first != k) {
            par = curr;
            if (k < curr->data.first) {
                curr = curr->left;
            }
            else {
                curr = curr->right;
            }
        }
        // не нашли
        if (!curr) {
            throw ("Node not found");
        }
        return curr;
    }
    void SetValue(const TKey& k, const TVal& v)
    {
        NodeType* curr = Search(k);
        curr->data.second = v;
    }
    void Walk(NodeType* r)
    {
        if (r) {
            if (r->left)
                Walk(r->left);
            cout << "Key:" << r->data.first;
            if (r->data.second)
                cout << ", value:" << r->data.second;
            if (r->right)
                cout << ", right:" << r->right->data.first;
            if (r->left)
                cout << ", left:" << r->left->data.first;
            if (r->parent)
                cout << ", parent:" << r->parent->data.first;
            cout << "\n";
            if (r->right)
                Walk(r->right);
        }
        else
            throw ("Tree is empty");

    }
    virtual NodeType* Insert(NodeType* tmp, const TKey& k) {
        if (isEmpty()) {
            tmp = new NodeType();
            tmp->data.first = k;
            return tmp;
        }
        else {
            if (tmp->data.first == k) {
                return tmp; //уже есть
            }
            if (k < tmp->data.first) {
                if (tmp->left == nullptr) {
                    tmp->left = new NodeType();
                    tmp->left->parent = tmp;
                    tmp->left->data.first = k;
                }
                else {
                    Insert(tmp->left, k);
                }
            }
            else {
                if (tmp->right == nullptr) {
                    tmp->right = new NodeType();
                    tmp->right->parent = tmp;
                    tmp->right->data.first = k;
                }
                else {
                    Insert(tmp->right, k);
                }
            }
            return tmp;
        }
    }
    NodeType* Suce(NodeType* x)
    {
        if (x->right != nullptr)
            return min(x->right);
        else {
            NodeType* tmp = x->parent;
            while (tmp->data.first < x->data.first)
            {
                x = tmp; tmp = x->parent;
            }
            return tmp;
        }
    }
    NodeType* Prev(NodeType* x)
    {
        if (x->left)
            return max(x->left);
        else {
            NodeType* tmp = x->parent;
            while (tmp && x == tmp->left)
            {
                x = tmp; tmp = x->parent;
                return tmp;
            }
        }
    }
    bool isEmpty() const { return root == nullptr; };
    virtual void Delete_node(const TKey& k)
    {
        if (isEmpty()) {
            throw ("Tree is empty");
        }
        // ищем звено
        NodeType* curr = root;
        NodeType* par = nullptr;
        while (curr && curr->data.first != k) {
            par = curr;
            if (k < curr->data.first) {
                curr = curr->left;
            }
            else {
                curr = curr->right;
            }
        }
        // не нашли
        if (!curr) {
            throw ("Node not found");
        }
        // нет потомков 
        if (!curr->left && !curr->right) {
            if (curr == root) {
                root = nullptr;
            }
            else if (par->left == curr) {
                par->left = nullptr;
            }
            else {
                par->right = nullptr;
            }
            delete curr;
        }
        // 1 потомок
        else if (!curr->left || !curr->right) {
            NodeType* child = (curr->left) ? curr->left : curr->right;

            if (curr == root) {
                root = child; // если удаляем корень - обновляем его
            }
            else if (par->left == curr) {
                par->left = child;
            }
            else {
                par->right = child;
            }
            delete curr;
        }
        // 2 потомка
        else {
            // наименьший справа
            NodeType* succ = curr->right;
            NodeType* succPar = curr;

            while (succ->left) {
                succPar = succ;
                succ = succ->left;
            }
            curr->data.first = succ->data.first;
            // удаляем successor 
            if (succPar->left == succ) {
                succPar->left = succ->right;
            }
            else {
                succPar->right = succ->right;
            }
            delete succ;
        }
    }
    virtual int GetHeight(NodeType* r) {
        if (r) {
            if (!(r->left) && !(r->right))
                return 0;
            int lg = 0, rg = 0;
            if (r->left)
                lg = GetHeight(r->left);
            else lg = 0;
            if (r->right)
                rg = GetHeight(r->right);
            else rg = 0;
            int k = std::max(lg, rg) + 1;
            // cout << r->data << " " << k << "\n";
            return k;
        }
        else
            return 0;
    }
protected:
    void Delete_tree(NodeType* tmp)
    {
        if (tmp)
        {
            Delete_tree(tmp->left);
            Delete_tree(tmp->right);
            delete tmp;
        }
    }
    NodeType* min(NodeType* x) {
        while (x->left) {
            x = x->left;
        }
        return x;
    }
    NodeType* max(NodeType* x) {
        while (x->right) {
            x = x->right;
        }
        return x;
    }
};

template <typename TKey, typename TVal>
class avlTree : public Tree<TKey, TVal, avlNode<TKey, TVal>> {
public:
    using NodeType = avlNode<TKey, TVal>;
    avlTree(const TKey& k) {
        this->root = new NodeType();
        this->root->data.first = k;
    }
};
