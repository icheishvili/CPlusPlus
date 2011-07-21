#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <sstream>
#include <algorithm>
#include "utils.hpp"
#include "node.hpp"
#include "graph.hpp"
#include "mongoose.h"

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

void* handle_similar_tracks_action(mg_event event, mg_connection* conn, const mg_request_info* request) {
    if (event == MG_NEW_REQUEST) {
        std::vector<std::pair<std::string, std::string> > params = parse_qs(request->query_string);
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

static void* http_callback(mg_event event, mg_connection* conn, const mg_request_info* request) {
    if (strcmp(request->uri, "/similar-tracks/") == 0 || strcmp(request->uri, "/similar-tracks") == 0) {
        return handle_similar_tracks_action(event, conn, request);
    }
    return NULL;
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
