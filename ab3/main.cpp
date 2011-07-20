#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <sstream>
#include <algorithm>
#include "mongoose.h"

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

std::vector<std::string> tokenize(std::string str, std::string delimiters) {
    std::vector<std::string> tokens;
    size_t lastPosition = str.find_first_not_of(delimiters, 0);
    size_t position = str.find_first_of(delimiters, lastPosition);

    while (std::string::npos != position || std::string::npos != lastPosition) {
        tokens.push_back(str.substr(lastPosition, position - lastPosition));
        lastPosition = str.find_first_not_of(delimiters, position);
        position = str.find_first_of(delimiters, lastPosition);
    }

    return tokens;
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

bool sortPairs(std::pair<std::string, unsigned int> a, std::pair<std::string, unsigned int> b) {
    if (a.second == b.second) {
        return a.first > b.first;
    } else {
        return a.second > b.second;
    }
}

std::vector<std::pair<std::string, unsigned short> > recommend(Graph* g, std::string nodeId) {
    Node* node = g->getNode(nodeId);
    std::vector<std::pair<std::string, unsigned short > > scoreList;
    if (!node) {
        return scoreList;
    }

    std::map<std::string, unsigned short> counts;
    std::vector<Node*>* charts = node->getNeighbors();
    std::vector<Node*>* tracks;
    std::map<std::string, unsigned short>::const_iterator countsIt;
    std::string relatedId;
    for (size_t i = 0; i < charts->size(); i++) {
        tracks = charts->at(i)->getNeighbors();
        for (size_t j = 0; j < tracks->size(); j++) {
            relatedId = tracks->at(j)->getId();
            if (relatedId == nodeId) {
                continue;
            }
            ++counts[relatedId];
        }
    }

    countsIt = counts.begin();
    while (countsIt != counts.end()) {
        scoreList.push_back(std::make_pair(countsIt->first, countsIt->second));
        ++countsIt;
    }

    std::sort(scoreList.begin(), scoreList.end(), sortPairs);

    return scoreList;
}

static Graph* graph;

std::vector<std::pair<std::string, std::string> > parse_qs(char* raw_query_string) {
    std::vector<std::pair<std::string, std::string> > parsed;
    if (!raw_query_string) {
        return parsed;
    }
    std::string query_string = raw_query_string;
    std::vector<std::string> params = tokenize(query_string, "&");
    std::vector<std::string> parts;
    std::string name;
    std::string value;
    for (size_t i = 0; i < params.size(); i++) {
        parts = tokenize(params.at(i), "=");
        name = parts.at(0);
        if (parts.size() > 1) {
            value = parts.at(1);
        } else {
            value = "";
        }
        parsed.push_back(std::make_pair(name, value));
    }
    return parsed;
}

std::string get_param_value(std::vector<std::pair<std::string, std::string> > params, std::string name, std::string default_value) {
    std::pair<std::string, std::string> param;
    for (size_t i = 0; i < params.size(); i++) {
        param = params.at(i);
        if (param.first == name) {
            return param.second;
        }
    }
    return default_value;
}

static void* http_callback(enum mg_event event, struct mg_connection* conn, const struct mg_request_info* request_info) {
    if (event == MG_NEW_REQUEST) {
        std::vector<std::pair<std::string, std::string> > params = parse_qs(request_info->query_string);
        std::string trackId = std::string("track-") + get_param_value(params, "trackId", "0");
        size_t limit = atoi(get_param_value(params, "limit", "24").c_str());
        std::vector<std::pair<std::string, unsigned short> > scoreList = recommend(graph, trackId);
        std::string linkTrackId;
        mg_printf(conn, "HTTP/1.1 200 OK\r\n");
        mg_printf(conn, "Content-Type: text/html\r\n\r\n");
        for (size_t i = 0; i < scoreList.size() && i < limit; i++) {
            linkTrackId = tokenize(scoreList.at(i).first, "-").at(1);
            mg_printf(conn, "%s,%d &nbsp;&nbsp;<a href=\"http://www.beatport.com/track/_/%s\">=></a><br/>\n", scoreList.at(i).first.c_str(), scoreList.at(i).second, linkTrackId.c_str());
        }
        return const_cast<char*>("");
    } else {
        return NULL;
    }
}

int main(int argc, char** argv) {
    graph = new Graph();

    for (unsigned short i = 1; i < argc; i++) {
        populateGraph(argv[i], graph);
    }

    struct mg_context *ctx;
    const char *options[] = {"listening_ports", "8080", NULL};

    ctx = mg_start(&http_callback, NULL, options);
    getchar();
    mg_stop(ctx);

    delete graph;
}
