#ifndef ProductsImpl_h
#define ProductsImpl_h

#include "AVL_Tree.h"


class ProductsImpl 
{
    private:
        AVL_Tree tree;

    public:
        ProductsImpl() {};
        ~ProductsImpl() {};
        //добавляет новый продукт
        // возвращает true - если продукта с таким id еще не было
        // возвращает false - если был такой id, вставка отменяется
        bool addProduct(Product product) 
        {
            return tree.Insert(product);
        };

        //Удаляет продукт
        // возвращает true - если продукт с таким id был
        // возвроащает false - если id не было, (удаления не происходит)
        bool deleteProduct(Product product) 
        {
            return tree.Remove(product.id);
        };


        // возращает name продукта у которого идентификатор равен (=) id
        // если продукта нет, вернуть пустую строку ""
        string getName(string id) 
        {
            return tree.SearchById(id);
        };


        //возвращает массив (список) идентификаторов (id)
        // У котрых наименование равно (=) name
        // Если таких нет, возвращается пустой массив (список)
        vector<string> findByName(string name) 
        {
            return tree.SearchByName(name);
        };
};

#endif