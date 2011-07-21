#include "node.hpp"

Node::Node(std::string id) {
    mId = id;
    mNeighbors = new std::vector<Node*>;
}

Node::~Node() {
    delete mNeighbors;
}

std::string Node::getId() {
    return mId;
}

std::vector<Node*>* Node::getNeighbors() {
    return mNeighbors;
}

void Node::addNeighbor(Node* node) {
    mNeighbors->push_back(node);
}

std::ostream& operator<<(std::ostream& os, Node* node) {
    os << "id: " << node->getId();
    os << ", neighbors: [";

    std::vector<Node*>::const_iterator it = node->mNeighbors->begin();
    while (it != node->mNeighbors->end()) {
        os << (*it)->getId() << ", ";
        ++it;
    }
    os << "]";
    return os;
}
