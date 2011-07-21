#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include "utils.hpp"
#include "node.hpp"

class Graph {
private:
    std::map<std::string, Node*>* mGraph;
public:
    Graph();
    ~Graph();
    bool addNode(Node*);
    Node* getNode(std::string);
    void addLink(std::string, std::string, bool);
    friend std::ostream& operator<<(std::ostream&, Graph*);
};

std::ostream& operator<<(std::ostream&, Graph*);
void populateGraph(std::string, Graph*);

#endif
