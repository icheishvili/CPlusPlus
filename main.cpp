#include <iostream>
#include <string>
#include "avl_tree.h"

using namespace std;

int main(int argc, char** argv) {
    AVLTree<string, string> tree;

    tree.insert("0", "zero");
    tree.insert("1", "one");
    tree.insert("2", "two");
    tree.insert("3", "three");
    tree.insert("4", "four");
    tree.insert("5", "five");
    tree.insert("6", "six");
    tree.insert("7", "seven");
    tree.insert("8", "eight");
    tree.insert("9", "nine");

    cout << tree.toString() << endl;

    return 0;
}

