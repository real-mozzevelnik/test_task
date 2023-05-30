#ifndef AVL_Tree_h
#define AVL_Tree_h

#include "Product.h"

#include <string>

typedef struct Node
{
    Product key;
    u_char height;
    Node *left, *right;
} Node;


class AVL_Tree
{
    public:
        Node *head;

    public:
        AVL_Tree()
        {
            head = nullptr;
        };
        
        ~AVL_Tree()
        {
            DelTree(head);
        };

        bool Insert(Product product)
        {
            bool was_inserted = true;
            head = InsertNode(head, product, &was_inserted);
            return was_inserted;
        };

        bool Remove(string product_id)
        {
            RemoveNode(head, stoi(product_id));
            return true;
        }

        void print_sim (Node* t, int tbl)
        {
            if (!t) return;
            if (t->right) print_sim(t->right, tbl+5);
            printf ("%*s(%d)\n", tbl, t->key.id.c_str(), t->height);
            if (t->left) print_sim(t->left, tbl+5);
        };

    private:
        void DelTree(Node *node)
        {
            if (!node) return;
            if (node->left) DelTree (node->left);
            if (node->right) DelTree (node->right);
            delete node;
        };

        int BalanceFactor(Node *node) 
        {
            return ( node->left ? node->left->height : 0 ) - (node->right ? node->right->height : 0 );
        };

        Node * CreateNode(Product product)
        {
            Node *n = new(nothrow) Node;
            if (!n)
            {
                cout << "Memory error in CreateNode func." << endl;
                exit(EXIT_FAILURE);
            }
            n->key = product;
            n->left = n->right = nullptr;
            n->height = 1;
            return n;
        };

        void FixHeight(Node *node)
        {
            unsigned char hl = node->left ? node->left->height : 0;
            unsigned char hr = node->right ? node->right->height : 0;
            node->height = ( hl>hr ? hl : hr ) + 1;
        };

        Node* RotateRight(Node* node)
        {
            Node* q = node->left;
            node->left = q->right;
            q->right = node;
            FixHeight(node);
            FixHeight(q);
            return q;
        };

        Node* RotateLeft (Node* node)
        {
            Node* q = node->right;
            node->right = q->left;
            q->left = node;
            FixHeight(node);
            FixHeight(q);
            return q;
        };

        Node* Balance (Node* node)
        {
            FixHeight(node);
            if (BalanceFactor(node) == -2)
            {
                if (BalanceFactor(node->right) > 0)
                    node->right = RotateRight(node->right);
                return RotateLeft(node);
            }
            if (BalanceFactor(node) == 2)
            {
                if (BalanceFactor(node->left) < 0)
                    node->left = RotateLeft(node->left);
                return RotateRight(node);
            }
            return node;
        };

        Node * InsertNode(Node* node, Product product, bool *was_inserted)
        {
            if (!node) 
            {
                return CreateNode(product);
            }
            if (node->key.id == product.id) 
            {
                *was_inserted = false;
                return node;
            }
            if (stoi(product.id) < stoi(node->key.id))
                node->left = InsertNode(node->left, product, was_inserted);
            else
                node->right = InsertNode(node->right, product, was_inserted);
            return Balance(node);
        };

        Node * FindMinNode(Node *node)
        {
            return node->left ? FindMinNode(node->left) : node;
        };

        Node * RemoveMinNode(Node *node)
        {
            if (!node->left)
		        return node->right;
	        node->left = RemoveMinNode(node->left);
	        return Balance(node);
        };

        Node * RemoveNode(Node *node, int product_id) // �������� ����� k �� ������ p
        {
            if (!node) return nullptr;
            if (product_id < stoi(node->key.id))
                node->left = RemoveNode(node->left ,product_id);
            else
            {
                if (product_id > stoi(node->key.id))
                    node->right = RemoveNode(node->right, product_id);
                else //  k == p->key
                {
                    Node* left_node = node->left;
                    Node* right_node = node->right;
                    delete node;
                    if (!right_node) 
                        return left_node;
                    Node* min = FindMinNode(right_node);
                    min->right = RemoveMinNode(right_node);
                    min->left = left_node;
                    return Balance(min);
                }
            }
            return Balance(node);
        };
};


#endif