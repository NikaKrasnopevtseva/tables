#pragma once
#include <iostream>
using namespace std;
#include <sstream>
template <typename TKey, typename TVal>
struct Node {
    Node* left;
    Node* right;
    Node* parent;
    pair <TKey, TVal> data;
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
struct avlNode : public Node<TKey, TVal> {
    int height;
};

template <typename TKey, typename TVal, typename NodeType>
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
                Walk(static_cast<NodeType*>(r->left));
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
                Walk(static_cast<NodeType*>(r->right));
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
                    Insert(static_cast<NodeType*>(tmp->left), k);
                }
            }
            else {
                if (tmp->right == nullptr) {
                    tmp->right = new NodeType();
                    tmp->right->parent = tmp;
                    tmp->right->data.first = k;
                }
                else {
                    Insert(static_cast<NodeType*>(tmp->right), k);
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
                curr = static_cast<NodeType*>(curr->left);
            }
            else {
                curr = static_cast<NodeType*>(curr->right);
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
            NodeType* child = static_cast<NodeType*>(curr->left) ? static_cast<NodeType*>(curr->left) : static_cast<NodeType*>(curr->right);

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
            NodeType* succ = static_cast<NodeType*>(curr->right);
            NodeType* succPar = curr;

            while (succ->left) {
                succPar = succ;
                succ = static_cast<NodeType*>(succ->left);
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
                lg = GetHeight(static_cast<NodeType*>(r->left));
            else lg = 0;
            if (r->right)
                rg = GetHeight(static_cast<NodeType*>(r->right));
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
            Delete_tree(static_cast<NodeType*>(tmp->left));
            Delete_tree(static_cast<NodeType*>(tmp->right));
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
    NodeType* Insert(NodeType* tmp, const TKey& k)  override {
        if (this->isEmpty()) {
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
                    Insert(static_cast<NodeType*>(tmp->left), k);
                }
            }
            else {
                if (tmp->right == nullptr) {
                    tmp->right = new NodeType();
                    tmp->right->parent = tmp;
                    tmp->right->data.first = k;
                }
                else {
                    Insert(static_cast<NodeType*>(tmp->right), k);
                }
            }
            Balance(static_cast<NodeType*>(tmp));
            return tmp;
        }
        // Обновляем высоту и проверяем баланс
        UpdateHeight(tmp);
        return Balance(tmp);
    }

    void Delete_node(const TKey& k) override {
        if (this->isEmpty()) {
            throw ("Tree is empty");
        }

        // Ищем узел для удаления
        NodeType* curr = this->root;
        NodeType* par = nullptr;

        while (curr && curr->data.first != k) {
            par = curr;
            if (k < curr->data.first) {
                curr = static_cast<NodeType*>(curr->left);
            }
            else {
                curr = static_cast<NodeType*>(curr->right);
            }
        }
        //узел не найден
        if (!curr) {
            throw ("Node not found");
        }
        // у узла нет потомков
        if (!curr->left && !curr->right) {
            if (curr == this->root) {
                this->root = nullptr;
            }
            else if (par->left == curr) {
                par->left = nullptr;
            }
            else {
                par->right = nullptr;
            }
            delete curr;
        }
        //один потомок
        else if (!curr->left || !curr->right) {
            NodeType* child = (curr->left) ? static_cast<NodeType*>(curr->left) : static_cast<NodeType*>(curr->right);

            if (curr == this->root) {
                this->root = child;
            }
            else if (par->left == curr) {
                par->left = child;
            }
            else {
                par->right = child;
            }

            if (child) {
                child->parent = par; // Обновляем родителя потомка
            }

            delete curr;
        }
        //два потомка
        else {
            // Находим наименьший узел в правом поддереве 
            NodeType* succ = static_cast<NodeType*>(curr->right);
            NodeType* succPar = curr;

            while (succ->left) {
                succPar = succ;
                succ = static_cast<NodeType*>(succ->left);
            }
            // Копируем данные из successor в текущий узел
            curr->data = succ->data;
            // Удаляем successor
            if (succPar->left == succ) {
                succPar->left = static_cast<NodeType*>(succ->right);
            }
            else {
                succPar->right = static_cast<NodeType*>(succ->right);
            }

            if (succ->right) {
                static_cast<NodeType*>(succ->right)->parent = succPar; // Обновляем родителя правого поддерева 
            }
            delete succ;
        }
        if (par) {
            UpdateHeight(par);
            Balance(par);
        }
    }
    NodeType* RotateLeft(NodeType* node) {
        NodeType* right = static_cast<NodeType*>(node->right);
        node->right = right->left;
        if (node->right != nullptr) {
            node->right->parent = node;
        }
        right->parent = node->parent;
        if (node->parent == nullptr) {
            this->root = right;
        }
        else if (node == node->parent->left) {
            node->parent->left = right;
        }
        else {
            node->parent->right = right;
        }
        right->left = node;
        node->parent = right;
        UpdateHeight(node);
        UpdateHeight(right);
        return right;
    }
    NodeType* RotateRight(NodeType* node) {
        NodeType* left = static_cast<NodeType*>(node->left);
        node->left = left->right;
        if (node->left != nullptr) {
            node->left->parent = node;
        }
        left->parent = node->parent;
        if (node->parent == nullptr) {
            this->root = left;
        }
        else if (node == node->parent->right) {
            node->parent->right = left;
        }
        else {
            node->parent->left = left;
        }
        left->right = node;
        node->parent = left;
        UpdateHeight(node);
        UpdateHeight(left);
        return left;
    }
    NodeType* Balance(NodeType* node) {
        if (node == nullptr) {
            return nullptr;
        }
        UpdateHeight(node);
        int balance = GetBalance(node);
        // Левый тяжелый
        if (balance > 1) {
            if (GetBalance(static_cast<NodeType*>(node->left)) >= 0) {
                return RotateRight(node);
            }
            else {
                node->left = RotateLeft(static_cast<NodeType*>(node->left));
                return RotateRight(node);
            }
        }
        // Правый тяжелый
        if (balance < -1) {
            if (GetBalance(static_cast<NodeType*>(node->right)) <= 0) {
                return RotateLeft(node);
            }
            else {
                node->right = RotateRight(static_cast<NodeType*>(node->right));
                return RotateLeft(node);
            }
        }
        return node;
    }
    void UpdateHeight(NodeType* node) {
        if (node == nullptr) {
            return;
        }
        int leftHeight = (node->left == nullptr) ? 0 : static_cast<NodeType*>(node->left)->height;
        int rightHeight = (node->right == nullptr) ? 0 : static_cast<NodeType*>(node->right)->height;
        node->height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
    }
    int GetBalance(NodeType* node) {
        if (node == nullptr) {
            return 0;
        }
        return GetHeight(static_cast<NodeType*>(node->left)) - GetHeight(static_cast<NodeType*>(node->right));
    }
    int GetHeight(NodeType* node)  override {
        return node == nullptr ? 0 : node->height;
    }
 };
