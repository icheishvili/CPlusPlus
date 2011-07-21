#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <string>
#include <iostream>

class Node {
private:
    std::string mId;
    std::vector<Node*>* mNeighbors;
public:
    Node(std::string);
    ~Node();
    std::string getId();
    std::vector<Node*>* getNeighbors();
    void addNeighbor(Node*);
    friend std::ostream& operator<<(std::ostream&, Node*);
};

std::ostream& operator<<(std::ostream&, Node*);

#endif
