#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <string>
#include "Graph.h"
#include <queue>
#include "../include/Tree.h"

using namespace std;

class Agent;

enum TreeType{
  Cycle,
  MaxRank,
  Root
};


class Session{
public:
    Session(const std::string& path);
    //Rule of 5:
    ~Session();
    Session(const Session &other); //copy constructor
    Session & operator=(const Session &other); //copy assignment operator
    Session(Session &&other, Graph g); //move constructor
    const Session& operator=(Session &&other); //move assignment operator

    void clear();
    void simulate();
    void addAgent(const Agent& agent);
    //void setGraph(const Graph& graph);
    Graph getGraph() const;
    void enqueueInfected(int);
    int dequeueInfected();
    bool infectedQueEmpty();
    TreeType getTreeType() const;
    void removeVirus();
    void addVirus(int nodeInd);
    int getCycle() const;
    void virusAct(int nodeInd);
    void contactTracerAct(int nodeToIsolate);
    //void isolate(int nodeInd);
    Session cycleTreeHelper();
private:
    Graph g;
    TreeType treeType;
    vector<Agent*> agents;
    queue<int> infectedQue;
    vector<int> infectedOutput;
    int cycle;
    int activeViruses; //TODO: Remove this. i changed simulate so this is not needed
    void setTreeType(const TreeType &other);

    //void virusAct();

    //void virusAct(Session);
};

#endif
