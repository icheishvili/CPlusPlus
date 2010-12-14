#ifndef _AVL_TREE_H
#define _AVL_TREE_H

#include <string>
#include "avl_node.h"

using namespace std;

/**
 * An implementation of the AVL tree.  The types that can be used for
 * the key and value are fully generic.  The public methods of this
 * class try to mimic those of other STL containers.  The only thing
 * not really provided is iterator support.
 */
template <class T, class U>
class AVLTree {
private:
    /**
     * The root of the tree.
     */
    AVLNode<T, U>* root;

    /**
     * Starting at a given node, deletes all children nodes and then
     * the given node.
     */
    void clear(AVLNode<T, U>*);

    /**
     * Deletes a single node, identified by its key.  The given node
     * is the start of the search path.
     */
    AVLNode<T, U>* erase(T, AVLNode<T, U>*);

    /**
     * Inserts a node into the tree at the ideal location and possibly
     * re-balances the tree.  The given node is the start of the
     * search path.
     */
    AVLNode<T, U>* insert(T, U, AVLNode<T, U>*);

    /**
     * Performs a single left rotation.  The technical explanation of
     * this can be found in the implementation file.
     */
    AVLNode<T, U>* singleRotateLeft(AVLNode<T, U>*);

    /**
     * Performs a single right rotation.  The technical explanation of
     * this can be found in the implementation file.
     */
    AVLNode<T, U>* singleRotateRight(AVLNode<T, U>*);

    /**
     * Performs a double left rotation.  The technical explanation of
     * this can be found in the implementation file.
     */
    AVLNode<T, U>* doubleRotateLeft(AVLNode<T, U>*);

    /**
     * Performs a double right rotation.  The technical explanation of
     * this can be found in the implementation file.
     */
    AVLNode<T, U>* doubleRotateRight(AVLNode<T, U>*);
public:
    /**
     * Constructor--initializes the tree.
     */
    AVLTree();

    /**
     * Destructor--frees all node memory.
     */
    ~AVLTree();

    /**
     * Returns a string represntation of the tree.
     */
    string toString();

    /**
     * Deletes all nodes in the tree.
     */
    void clear();

    /**
     * Deletes the specified node in the tree, identified by the key.
     * A re-balance of the tree may occur.
     */
    void erase(T key);

    /**
     * Finds the specified node in the tree, identified by the key.
     */
    U find(T key);

    /**
     * Inserts the given key and data as a node into the tree.  A
     * re-balance may occur.
     */
    void insert(T key, U data);
};


template <class T, class U>
AVLTree<T, U>::AVLTree() {
    this->root = NULL;
}

template <class T, class U>
AVLTree<T, U>::~AVLTree() {
    this->clear();
}

template <class T, class U>
string AVLTree<T, U>::toString() {
    if (this->root) {
        return this->root->toString();
    } else {
        return "";
    }
}

/**
 * Rebalance operation:
 *
 *             +-----+                +-----+
 *             |  5  |                |  4  |
 *             +-----+                +-----+
 *               / \                    /   \
 *              /   \                  /     \
 *             /     \                /       \
 *        +-----+     D           +-----+   +-----+
 *        |  4  |                 |  3  |   |  5  |
 *        +-----+                 +-----+   +-----+
 *          / \            =>       / \       / \
 *         /   \                   /   \     /   \
 *        /     \                 /     \   /     \
 *   +-----+     C               A       B C       D
 *   |  3  |
 *   +-----+
 *     / \
 *    /   \
 *   /     \
 *  A       B
 */
template <class T, class U>
AVLNode<T, U>* AVLTree<T, U>::singleRotateLeft(AVLNode<T, U>* node) {
    AVLNode<T, U>* left = node->left;
    node->left = left->right;
    left->right = node;

    return left;
}

/**
 * Rebalance operation:
 *
 *    +-----+                       +-----+
 *    |  3  |                       |  4  |
 *    +-----+                       +-----+
 *     / \                           /  \
 *    /   \                         /    \
 *   /     \                       /      \
 *  A    +-----+               +-----+   +-----+
 *       |  4  |               |  3  |   |  5  |
 *       +-----+               +-----+   +-----+
 *         / \          =>       / \       / \
 *        /   \                 /   \     /   \
 *       /     \               /     \   /     \
 *      B    +-----+          A       B C       D
 *           |  5  |
 *           +-----+
 *             / \
 *            /   \
 *           /     \
 *          C       D
 */
template <class T, class U>
AVLNode<T, U>* AVLTree<T, U>::singleRotateRight(AVLNode<T, U>* node) {
    AVLNode<T, U>* right = node->right;
    node->right = right->left;
    right->left = node;

    return right;
}

/**
 * Rebalance operation:
 *
 *          +-----+              +-----+
 *          |  5  |              |  4  |
 *          +-----+              +-----+
 *            / \                  /   \
 *           /   \                /     \
 *          /     \              /       \
 *    +-----+      D         +-----+   +-----+
 *    |  3  |                |  3  |   |  5  |
 *    +-----+                +-----+   +-----+
 *      / \            =>      / \       / \
 *     /   \                  /   \     /   \
 *    /     \                /     \   /     \
 *   /       \              A       B C       D
 *  A     +-----+
 *        |  4  |
 *        +-----+
 *          / \
 *         /   \
 *        /     \
 *       B       C
 */
template <class T, class U>
AVLNode<T, U>* AVLTree<T, U>::doubleRotateLeft(AVLNode<T, U>* node) {
    AVLNode<T, U>* five = node;
    AVLNode<T, U>* three = five->left;
    AVLNode<T, U>* four = three->right;
    AVLNode<T, U>* b = four->left;
    AVLNode<T, U>* c = four->right;

    three->right = b;
    five->left = c;
    four->left = three;
    four->right = five;

    return four;
}

/**
 * Rebalance operation:
 *
 *    +-----+                 +-----+
 *    |  3  |                 |  4  |
 *    +-----+                 +-----+
 *      / \                     /   \
 *     /   \                   /     \
 *    /     \                 /       \
 *   A    +-----+         +-----+   +-----+
 *        |  5  |         |  3  |   |  5  |
 *        +-----+         +-----+   +-----+
 *          / \      =>     / \       / \
 *         /   \           /   \     /   \
 *        /     \         /     \   /     \
 *   +-----+     D       A       B C       D
 *   |  4  |
 *   +-----+
 *     / \
 *    /   \
 *   /     \
 *  B       C
 */
template <class T, class U>
AVLNode<T, U>* AVLTree<T, U>::doubleRotateRight(AVLNode<T, U>* node) {
    AVLNode<T, U>* three = node;
    AVLNode<T, U>* five = three->right;
    AVLNode<T, U>* four = five->left;
    AVLNode<T, U>* b = four->left;
    AVLNode<T, U>* c = four->right;

    three->right = b;
    five->left = c;
    four->left = three;
    four->right = five;

    return four;
}

template <class T, class U>
AVLNode<T, U>* AVLTree<T, U>::insert(T key, U data, AVLNode<T, U>* node) {
    if (!node) {
        AVLNode<T, U>* insertion = new AVLNode<T, U>(key, data);
        node = insertion;
    } else if (key < node->key) {
        node->left = this->insert(key, data, node->left);
        if (node->factor() == 2) {
            if (node->left->factor() >= 0) {
                node = this->singleRotateLeft(node);
            } else if (node->left->factor() == -1) {
                node = this->doubleRotateLeft(node);
            }
        }
    } else if (key > node->key) {
        node->right = this->insert(key, data, node->right);
        if (node->factor() == -2) {
            if (node->right->factor() <= 0) {
                node = this->singleRotateRight(node);
            } else if (node->right->factor() == 1) {
                node = this->doubleRotateRight(node);
            }
        }
    } else {
        node->data = data;
    }

    return node;
}

template <class T, class U>
U AVLTree<T, U>::find(T key) {
    AVLNode<T, U>* node = this->root;
    while (node) {
        if (key < node->key) {
            node = node->left;
        } else if (key > node->key) {
            node = node->right;
        } else {
            return node->data;
        }
    }

    return NULL;
}

template <class T, class U>
void AVLTree<T, U>::insert(T key, U data) {
    this->root = this->insert(key, data, this->root);
}

template <class T, class U>
AVLNode<T, U>* AVLTree<T, U>::erase(T key, AVLNode<T, U>* node) {
    if (node) {
        if (key < node->key) {
            node->left = this->erase(key, node->left);
            if (node->factor() == -2) {
                node = this->singleRotateRight(node);
            }
        } else if (key > node->key) {
            node->right = this->erase(key, node->right);
            if (node->factor() == 2) {
                node = this->singleRotateLeft(node);
            }
        } else {
            AVLNode<T, U>* child;
            AVLNode<T, U>* parent = node;
            bool looped = false;

            if (node->left) {
                child = node->left;
                while (child->right) {
                    looped = true;
                    parent = child;
                    child = child->right;
                }

                if (looped) {
                    parent->right = NULL;
                } else {
                    parent->left = NULL;
                }

                child->left = node->left;
                child->right = node->right;
                delete node;
                node = child;
            } else if (node->right) {
                child = node->right;
                while (child->left) {
                    looped = true;
                    parent = child;
                    child = child->left;
                }

                if (looped) {
                    parent->left = NULL;
                } else {
                    parent->right = NULL;
                }

                child->left = node->left;
                child->right = node->right;
                delete node;
                node = child;
            } else {
                delete node;
                node = NULL;
            }
        }
    }

    return node;
}

template <class T, class U>
void AVLTree<T, U>::erase(T key) {
    this->root = this->erase(key, this->root);
}

template <class T, class U>
void AVLTree<T, U>::clear(AVLNode<T, U>* node) {
    if (!node) {
        return;
    }

    if (node->left) {
        this->clear(node->left);
    }
    if (node->right) {
        this->clear(node->right);
    }

    delete node;
}

template <class T, class U>
void AVLTree<T, U>::clear() {
    if (this->root) {
        this->clear(this->root);
        this->root = NULL;
    }
}

#endif

