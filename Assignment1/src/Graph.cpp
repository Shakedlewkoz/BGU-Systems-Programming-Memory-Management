#include "../include/Graph.h"
#include "../include/Session.h"
#include <utility>
#include <vector>
#include <iostream>

Graph::Graph(vector<vector<int>> matrix) : edges((matrix)), infectedVec() {
    this->infectedVec = vector<int>(edges.size(), 0);
    /* 0 - healthy
     * 1 - carry
     * 2 - infected
     * */
}

int Graph::NumOfVer() { return edges.size(); }

void Graph::infectNode(int nodeInd) {
    infectedVec[nodeInd] = 2;
}

void Graph::setNodeStatus(int nodeInd, int status) {
    infectedVec[nodeInd] = status;
}

int Graph::getNodeStatus(int nodeInd) {
    return infectedVec[nodeInd];
}

vector<int> Graph::FindNeighbors(int nodeInd) {
    vector<int> neighbors;
    for (int i = 0; i < (int) edges.size(); i++) {
        if (edges[nodeInd][i] == 1) {
            neighbors.push_back(i);
        }
    }
    return neighbors;
}

void Graph::isolate(int nodeInd) {
    for (int i = 0; i < this->NumOfVer(); ++i) {
        edges[nodeInd][i] = 0;
        edges[i][nodeInd] = 0;
    }
}

vector<vector<int>> Graph::getEdges() {
    return edges;
};

