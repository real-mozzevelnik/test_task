#include "AVL_Tree.h"

int main(int argc, char **argv)
{
    AVL_Tree tree;
    Product p;
    p.id = "4";
    p.name = "ooo";
    cout << tree.Insert(p) << endl;
    p = {"3", " ppp"};
    cout << tree.Insert(p) << endl;
    tree.print_sim(tree.head, 5);
    getchar(), getchar();
        system("clear");

    tree.Remove(p.id);
        tree.print_sim(tree.head, 5);



}