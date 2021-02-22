#include <iostream>
#include "../include/Session.h"
#include "../include/json.hpp"
#include "fstream"


using json = nlohmann::json;
using namespace std;

class Agent;

Session::Session(const string &path) : g(vector<vector<int >>()), treeType(), agents(), infectedQue(), infectedOutput(),
                                       cycle(0),
                                       activeViruses(0) {
    ifstream i(path);
    json j; // creating a new json file named j
    i >> j; // json file told me to

    /*making the json graph file*/
    vector<vector<int>> matrix = j["graph"];
    g = Graph(matrix);
    for (auto &elem: j["agents"]) {
        if (elem[0] == "V") {
            Virus temp(elem[1]);
            addAgent(temp); //adding the virus to the agents list
            g.setNodeStatus(elem[1], 1);
            infectedOutput.push_back(elem[1]);
        } else {
            ContactTracer temp;
            addAgent(temp); //adding the CT to the agents
        }
    }

    /*setting the treeType*/
    if (j["tree"] == "M")
        treeType = MaxRank;
    else if (j["tree"] == "R")
        treeType = Root;
    else
        treeType = Cycle;

}

Session::~Session() { clear(); }

//copy constructor:
Session::Session(const Session &other) : g(other.g),
                                         treeType(other.treeType), agents(other.agents),
                                         infectedQue(other.infectedQue), infectedOutput(other.infectedOutput),
                                         cycle(other.cycle),
                                         activeViruses(other.activeViruses) {
}

//copy assignment operator:
Session &Session::operator=(const Session &other) {
    if (this != &other) {
        this->clear();
        this->treeType = other.treeType;
        for (size_t i = 0; i < other.agents.size(); ++i) {
            agents.push_back(other.agents[i]->clone());
        }
        this->infectedQue = other.infectedQue;
        this->infectedOutput = other.infectedOutput;
        this->cycle = other.cycle;
        this->activeViruses = other.activeViruses;
    }
    return *this;
}

Session::Session(Session &&other, Graph g) : g(g), treeType(other.treeType), agents(other.agents),
                                             infectedQue(other.infectedQue), infectedOutput(other.infectedOutput),
                                             cycle(other.cycle),
                                             activeViruses(other.activeViruses) {
    other.clear();
}

//move assignment operator:
const Session &Session::operator=(Session &&other) {
    if (this != &other) {
        this->clear();
        this->treeType = other.treeType;
        for (size_t i = 0; i < other.agents.size(); ++i)
            agents.push_back(other.agents[i]->clone());
        this->infectedQue = other.infectedQue;
        this->infectedOutput = other.infectedOutput;
        this->cycle = other.cycle;
        this->activeViruses = other.activeViruses;
        other.clear();
    }
    return *this;
}

void Session::clear() {
    for (size_t i = 0; i < agents.size(); ++i) {
        delete agents[i];
        agents[i] = nullptr;
    }
    agents.clear();
    while (!infectedQue.empty()) { infectedQue.pop(); }
    infectedOutput.clear();
    this->cycle = 0;
    this->activeViruses = 0;
}


void Session::simulate() {
    unsigned int agentSize = agents.size();
    /* because my stop condition is when it has been a complete cycle and no agent was added
     * i need a condition that will let the loop get in the first time*/

    while (agents.size() > agentSize || cycle == 0) {
        /* we will activate all of the agents on the agents list by order */
        agentSize = agents.size();
        for (unsigned int i = 0; i < agentSize; ++i)
            agents[i]->act(*this);
        cycle++;

    }
    /*make the output json file*/
    json output;
    output["graph"] = g.getEdges();
    output["infected"] = infectedOutput;
    std::ofstream file("./output.json");

    file << output;
}


void Session::addAgent(const Agent &agent) {
    agents.push_back(agent.clone());
}

Graph Session::getGraph() const {
    return g;
}


void Session::enqueueInfected(int infectedNode) {
    infectedQue.push(infectedNode);
}

int Session::dequeueInfected() {
    int temp = infectedQue.front();
    return temp;
}

bool Session::infectedQueEmpty() {
    return infectedQue.empty();
}


void Session::setTreeType(const TreeType &other) {
    this->treeType = other;

}

TreeType Session::getTreeType() const {
    return treeType;
}

void Session::addVirus(int nodeInd) {
    if (g.getNodeStatus(nodeInd) == 1)
        infectedOutput.push_back(nodeInd);
}

int Session::getCycle() const { return cycle; }


void Session::virusAct(int nodeInd) {
    switch (g.getNodeStatus(nodeInd)) {
        case 0: //got a healthy node
            return;
        case 1: //got a carry node
            g.setNodeStatus(nodeInd, 2);
            this->enqueueInfected(nodeInd);
            break;
        case 2: //got an infected node
            break;
    }
    vector<int> neighbours = g.FindNeighbors(nodeInd);//getting a list of the nodes neighbours.

    for (int neighbour : neighbours) { //for (int i = 0; i < neighbours.size(); i++)
        if (g.getNodeStatus(neighbour) == 0) {
            g.setNodeStatus(neighbour, 1);
            this->addVirus(neighbour); // adding a virus to the virus counter.
            this->addAgent(Virus(neighbour)); //adding a Virus to the agent list (carrier)
            break;
        }
    }
}

void Session::contactTracerAct(int nodeToIsolate) {
    g.isolate(nodeToIsolate);
    infectedQue.pop();
}








