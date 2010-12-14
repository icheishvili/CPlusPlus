#ifndef _AVL_NODE_H
#define _AVL_NODE_H

#include <string>
#include <sstream>

using namespace std;

/**
 * A container structure used as the building block of the AVL tree.
 */
template <class T, class U>
class AVLNode {
private:
    /**
     * Takes a node and recursively traverses it post-order to
     * generate a string representation of it and all children.
     */
    string toString(AVLNode<T, U>*, string);
public:
    /**
     * Pretty public interface for the private version of this method.
     * Starts the recursive loop off.
     */
    string toString();

    /**
     * The node to the left of this node.
     */
    AVLNode *left;

    /**
     * The node to the right of this node.
     */
    AVLNode *right;

    /**
     * The key of the node.
     */
    T key;

    /**
     * The value of the node.
     */
    U data;

    /**
     * The constructor initializes the key, value, and all pointers.
     */
    AVLNode(T, U);

    /**
     * Computes the difference between the height of the left subtree
     * and the height of the right subtree.  This is used to know when
     * to re-balance the tree.
     */
    int factor();

    /**
     * Gets the height of this tree (1 + max(left, right)).
     */
    unsigned int height();
};

template <class T, class U>
AVLNode<T, U>::AVLNode(T key, U data) {
    this->left = NULL;
    this->right = NULL;
    this->key = key;
    this->data = data;
}

template <class T, class U>
int AVLNode<T, U>::factor() {
    unsigned int leftHeight = 0;
    unsigned int rightHeight = 0;

    if (this->left) {
        leftHeight = this->left->height();
    }
    if (this->right) {
        rightHeight = this->right->height();
    }

    return leftHeight - rightHeight;
}

template <class T, class U>
unsigned int AVLNode<T, U>::height() {
    unsigned int height = 1;
    unsigned int leftHeight = 0;
    unsigned int rightHeight = 0;

    if (this->left) {
        leftHeight = this->left->height();
    }
    if (this->right) {
        rightHeight = this->right->height();
    }

    if (leftHeight > rightHeight) {
        return height + leftHeight;
    } else {
        return height + rightHeight;
    }
}

template <class T, class U>
string AVLNode<T, U>::toString(AVLNode<T, U>* node, string indent) {
    stringstream ss;

    indent += "    ";
    if (node && node->right) {
        ss << this->toString(node->right, indent);
    } else {
        ss << indent << "" << endl;
    }

    indent = indent.substr(0, indent.size() - 4);
    if (node) {
        ss << indent << "" << node->key << " => " << node->data << "";
    } else {
        ss << indent << "" << endl;
    }

    indent += "    ";
    if (node && node->left) {
        ss << this->toString(node->left, indent);
    } else {
        ss << indent << "" << endl;
    }

    return ss.str();
}

template <class T, class U>
string AVLNode<T, U>::toString() {
    return this->toString(this, "");
}

#endif
