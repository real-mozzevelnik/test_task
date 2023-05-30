#include "ProductsImpl.h"
#include <assert.h>

int main(int argc, char **argv)
{
    ProductsImpl p;

    // Проводим тесты.

    assert(p.deleteProduct({"9", "k"}) == false);

    assert(p.addProduct({"0", "hello"}) == true);
    assert(p.addProduct({"1", "bye"}) == true);
    assert(p.addProduct({"2", "hello"}) == true);
    assert(p.addProduct({"3", "sea"}) == true);
    assert(p.addProduct({"4", "hello"}) == true);
    assert(p.addProduct({"5", "tea"}) == true);
    assert(p.addProduct({"6", "hello"}) == true);
    assert(p.addProduct({"7", "map"}) == true);
    assert(p.addProduct({"4", "hello"}) == false);

    assert(p.getName("8888") == "");
    assert(p.getName("4") == "hello");
    assert(p.getName("-322") == "");
    assert(p.getName("5") == "tea");

    vector<string> test;
    test.push_back("0");
    test.push_back("2");
    test.push_back("4");
    test.push_back("6");

    assert(p.findByName("hello") == test);

    test.clear();
    assert(p.findByName("rock") == test);

    assert(p.deleteProduct({"0", "hello"}) == true);
    assert(p.deleteProduct({"32", "eight"}) == false);

    cout << "Tests passed" << endl;
}