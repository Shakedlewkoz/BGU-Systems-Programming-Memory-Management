#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <string>
#include "Agent.h"

using namespace std;
class Session;
class Graph{
public:
    Graph(std::vector<std::vector<int>> matrix);
    int NumOfVer();
    void infectNode(int nodeInd);
    void setNodeStatus(int nodeInd, int status);
    int getNodeStatus(int nodeInd);
    vector<int> FindNeighbors(int nodeInd);
    void isolate(int nodeInd);
    vector<vector<int>> getEdges();
private:
    vector<vector<int>> edges;
    vector<int> infectedVec;
};

#endif
