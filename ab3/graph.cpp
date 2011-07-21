#include "graph.hpp"

Graph::Graph() {
    mGraph = new std::map<std::string, Node*>;
}

Graph::~Graph() {
    std::map<std::string, Node*>::const_iterator it = mGraph->begin();
    while (it != mGraph->end()) {
        delete it->second;
        ++it;
    }
    delete mGraph;
}

bool Graph::addNode(Node* node) {
    std::pair<std::string, Node*> value(node->getId(), node);
    std::pair<std::map<std::string, Node*>::const_iterator, bool> ret = mGraph->insert(value);
    if (!ret.second) {
        delete node;
    }
    return ret.second;
}

Node* Graph::getNode(std::string id) {
    std::map<std::string, Node*>::const_iterator it;
    it = mGraph->find(id);
    if (it != mGraph->end()) {
        return it->second;
    }
    return NULL;
}

void Graph::addLink(std::string one, std::string two, bool directed) {
    Node* nodeOne = getNode(one);
    if (!nodeOne) {
        return;
    }
    Node* nodeTwo = getNode(two);
    if (!nodeTwo) {
        return;
    }

    nodeOne->addNeighbor(nodeTwo);
    if (!directed) {
        nodeTwo->addNeighbor(nodeOne);
    }
}

std::ostream& operator<<(std::ostream& os, Graph* graph) {
    std::map<std::string, Node*>::const_iterator it = graph->mGraph->begin();
    while (it != graph->mGraph->end()) {
        os << it->second << std::endl;
        ++it;
    }
    return os;
}

void populateGraph(std::string filename, Graph* graph) {
    std::string line;
    std::ifstream input(filename.c_str());
    Node* left;
    Node* right;
    if (input.is_open()) {
        while (input.good()) {
            getline(input, line);
            if (line.size() < 1) {
                continue;
            }

            std::vector<std::string> tokens = tokenize(line, " ");
            if (tokens.size() < 3 || tokens.at(1) != "=>") {
                std::cerr << "Invalid format: '" << line << "'" << std::endl;
            } else {
                left = new Node(tokens.at(0));
                right = new Node(tokens.at(2));
                graph->addNode(left);
                graph->addNode(right);
                graph->addLink(tokens.at(0), tokens.at(2), false);
            }
        }
    } else {
        std::cerr << "Could not open input file." << std::endl;
    }
    input.close();
}
