#ifndef AVL_Tree_h
#define AVL_Tree_h

#include "Product.h"

#include <string>
#include <vector>

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
            bool found = true;
            RemoveNode(head, stoi(product_id), &found);
            return found;
        };

        // Получение имени продукта по id.
        string SearchById(string id)
        {
            return TraversalById(head, stoi(id));
        };

        // Получение всех id с заданным именем.
        vector<string> SearchByName(string name)
        {
            vector<string> id_array;
            DirectTraversalByName(head, name, &id_array);
            return id_array;
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

        // Рекурсивный метод нахождения узла с наименьшим ключом.
        Node * FindMinNode(Node *node)
        {
            return node->left ? FindMinNode(node->left) : node;
        };

        // Вытаскиваем узел с наименьшим ключом для удаления.
        Node * RemoveMinNode(Node *node)
        {
            if (!node->left)
		        return node->right;
	        node->left = RemoveMinNode(node->left);
	        return Balance(node);
        };

        // Рекурсивный метод удаления узла.
        Node * RemoveNode(Node *node, int product_id, bool *found)
        {
            // Если узел не найден.
            if (!node) 
            {  
                // Сообщаем об этом.
                *found = false;
                return nullptr;
            }
            // Если искомый ключ меньше - идем дальше влево.
            if (product_id < stoi(node->key.id))
                node->left = RemoveNode(node->left, product_id, found);
            else
            {
                // Если искомы ключ больше - идем вправо.
                if (product_id > stoi(node->key.id))
                    node->right = RemoveNode(node->right, product_id, found);
                // Если нужный узел найден - удаляем.
                else
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
            // Выполняем балансировку.
            return Balance(node);
        };

        // Рекурсивный метод обхода дерева для поиска по id.
        string TraversalById(Node* node, int id)
        {
            // Если нужный узел не найден - возвращаем пустую строку.
            if (!node)
                return "";
            // Если узел найден - возвращаем данные.
            if (stoi(node->key.id) == id)
                return node->key.name;
            // Иначе продолжаем поиск.
            else if (stoi(node->key.id) < id)
                return TraversalById(node->right, id);
            else
                return TraversalById(node->left, id);
        };

        // Рекурсивный метод для прямого обхода дерева для поиска по имени.
        // Поскольку дерево отсортировано по id, необходимо проходить по всему дереву.
        void DirectTraversalByName(Node *node, string name, vector<string> *id_array)
        {
            if (!node)
                return;
            if (node->key.name == name)
                id_array->push_back(node->key.id);
            DirectTraversalByName(node->left, name, id_array);
            DirectTraversalByName(node->right, name, id_array);
        };

};


#endif