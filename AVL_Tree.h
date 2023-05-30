#ifndef AVL_Tree_h
#define AVL_Tree_h

#include "Product.h"

#include <string>

// Структура, описывающая ноду дерева.
// key - содержащаяся внутри структура Product
// height - высота конкретной ноды.
// left, right - левый и правый потомки.
typedef struct Node
{
    Product key;
    u_char height;
    Node *left, *right;
} Node;


// Класс, описывающий АВЛ-дерево.
// Остается сбалансированным после каждой операции,
// таким образом асимптотическая оценка поиска по дереву - O(logn).
// Благодаря связной структуре хранения, удаление и добавление узлов выполняется также за O(logn)
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
            // Удаление всех узлов дерева.
            DelTree(head);
        };
        
        // Добавление нового узла в дерево.
        // Возвращает: 
        // true - узел добавлен
        // false - узел не добавлен.
        bool Insert(Product product)
        {
            // Проверяем был ли добавлен узел.
            bool was_inserted = true;
            head = InsertNode(head, product, &was_inserted);
            return was_inserted;
        };

        // Удаление узла по id.
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
        // Рекурсивный метод удаления всех узлов дерева.
        void DelTree(Node *node)
        {
            if (!node) return;
            if (node->left) DelTree (node->left);
            if (node->right) DelTree (node->right);
            delete node;
        };

        // Нахождение разницы высот левого и правого поддерева узла.
        int BalanceFactor(Node *node) 
        {
            return ( node->left ? node->left->height : 0 ) - (node->right ? node->right->height : 0 );
        };

        // Создание новой ноды.
        Node * CreateNode(Product product)
        {
            Node *n = new(nothrow) Node;
            // Проверка выделения памяти.
            if (!n)
            {
                cout << "Memory error in CreateNode func." << endl;
                // Если память не выделилась - удаляем дерево и завершаем программу.
                DelTree(head);
                exit(EXIT_FAILURE);
            }
            n->key = product;
            n->left = n->right = nullptr;
            n->height = 1;
            return n;
        };

        // Расчет высоты для узла.
        void FixHeight(Node *node)
        {
            unsigned char hl = node->left ? node->left->height : 0;
            unsigned char hr = node->right ? node->right->height : 0;
            // Выбираем наибольшую высоту из высот поддеревьев узла 
            // и добавляем единицу (высота узла-родителя на 1 больше).
            node->height = ( hl>hr ? hl : hr ) + 1;
        };

        // Правый поворот для балансировки дерева.
        Node* RotateRight(Node* node)
        {
            Node* q = node->left;
            node->left = q->right;
            q->right = node;
            FixHeight(node);
            FixHeight(q);
            return q;
        };

        // Левый поворот для балансировки дерева.
        Node* RotateLeft (Node* node)
        {
            Node* q = node->right;
            node->right = q->left;
            q->left = node;
            FixHeight(node);
            FixHeight(q);
            return q;
        };

        // Балансировка дерева.
        Node* Balance (Node* node)
        {
            // Пересчитываем высоту.
            FixHeight(node);
            // В зависимости от разницы высот поддеревьев выполняем нужный поворот.
            if (BalanceFactor(node) == -2)
            {
                // При необходимости выполняем большой поворот.
                if (BalanceFactor(node->right) > 0)
                    node->right = RotateRight(node->right);
                return RotateLeft(node);
            }
            if (BalanceFactor(node) == 2)
            {
                // При необходимости выполняем большой поворот.
                if (BalanceFactor(node->left) < 0)
                    node->left = RotateLeft(node->left);
                return RotateRight(node);
            }
            return node;
        };

        // Рекурсивный метод для добавления нового узла. 
        Node * InsertNode(Node* node, Product product, bool *was_inserted)
        {
            // Если на нужном месте узла не существует - создаем.
            if (!node) 
            {
                return CreateNode(product);
            }
            // Если находим узел с таким же ключем.
            if (node->key.id == product.id) 
            {
                // Отказываем в добавлении.
                *was_inserted = false;
                return node;
            }
            // Рекурсивно запускаем метод.
            if (stoi(product.id) < stoi(node->key.id))
                node->left = InsertNode(node->left, product, was_inserted);
            else
                node->right = InsertNode(node->right, product, was_inserted);

            // Выполняем балансировку.
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

        Node * RemoveNode(Node *node, int product_id)
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