#include "../include/Tree.h"
#include "../include/Session.h"

/**
 * This tree wil represent the shortest path tree in the graph from some starting node.
 * @param rootLabel - the starting node
 */
Tree::Tree(int rootLabel) : node(rootLabel), children() {}


Tree::~Tree() { clear(); }

void Tree::clone(const Tree &other) {
    if (!other.getChildren().empty()) {
        for (unsigned int i = 0; i < other.getChildren().size(); ++i) {
            *children[i] = *(other.getChildren()[i]);
            children[i]->node = other.node;
            children[i]->clone(*(other.getChildren()[i]));
        }
    }
}

//copy constructor:
Tree::Tree(const Tree &other) : node(other.node), children() {
    this->clone(other);
}

//copy assignment operator:
Tree &Tree::operator=(const Tree &other) {
    if (this == &other) {
        return *this;
    }
    this->clear();
    this->clone(other);
    this->node = other.node;
    return *this;
}





////copy constructor:
//Tree::Tree(const Tree &other) : node(other.node), children(other.children) {
//}
//
////copy assignment operator:
//Tree &Tree::operator=(const Tree &other) {
//    if (this == &other) {
//        return *this;
//    }
//    this->clear();
//    this->children = other.children;
//    this->node = other.node;
//    return *this;
//}

//move constructor:
Tree::Tree(Tree &&other) : node(other.node), children() {
    this->clone(other);
    other.clear();
}

//move assignment operator:
const Tree &Tree::operator=(Tree &&other) {
    if (this != &other) {
        this->clear();
        this->clone(other);
        this->node = other.node;
        other.clear();
    }
    return *this;
}


Tree *Tree::createTree(const Session &session, int rootLabel) {
    if (session.getTreeType() == Cycle) {
        CycleTree *cycleT;
        cycleT = new CycleTree(rootLabel, session.getCycle());
        return cycleT;
    } else if (session.getTreeType() == MaxRank) {
        MaxRankTree *maxRankT;
        maxRankT = new MaxRankTree(rootLabel);
        return maxRankT;
    } else {
        RootTree *rootT;
        rootT = new RootTree(rootLabel);
        return rootT;
    }
}


void Tree::addChildHelper(Tree *child) {
    children.push_back(child);
}

Tree *Tree::bfs(const Session &session, int rootLabel) {
    vector<vector<int>> edges = session.getGraph().getEdges();
    vector<bool> visited(session.getGraph().NumOfVer(), false);
    visited[rootLabel] = true;
    queue<Tree *> queue;
    Tree *outputRoot = createTree(session, rootLabel);
    queue.push(outputRoot);
    while (!queue.empty()) {
        Tree *temp = queue.front();
        queue.pop();
        vector<int> neighbors = session.getGraph().FindNeighbors(temp->node);
        for (int neighbor : neighbors) {
            if (!visited[neighbor]) {
                Tree *child = createTree(session, neighbor);
                temp->addChildHelper(child);
                queue.push(child);
                visited[neighbor] = true;
            }
        }
    }
    return outputRoot;
}

vector<Tree *> Tree::getChildren() const { return children; }

void Tree::clear() {
    if (!children.empty()) {
        for (unsigned int i = 0; i < children.size(); ++i) {
            children[i]->clear();
            delete children[i];
            children[i] = nullptr;
        }
    }
    children.clear();
}

int Tree::returnNode() {
    return node;
}

//need to add createGraph for all the constructors after we get session
CycleTree::CycleTree(int rootLabel, int currCycle) : Tree(rootLabel), currCycle(currCycle) {};

//returning the number of the c'th child of the root from the left (or less then c if there isnt)
int CycleTree::traceTree() {
    int currentTreeNode = this->returnNode();
    vector<Tree *> children = this->getChildren();
    for (int i = 0; i < currCycle && !children.empty(); ++i) {
        currentTreeNode = children[0]->returnNode();
        children = children[0]->getChildren();
    }
    return currentTreeNode;
}

MaxRankTree::MaxRankTree(int rootLabel) : Tree(rootLabel) {};

//find the node with the maximum number of children, if there is more than one it goes in order (lowest label, lowest num)
int MaxRankTree::traceTree() {
    Tree *max = this;
    queue<Tree *> q;
    q.push(this);
    Tree *temp;
    while (!q.empty()) {
        temp = q.front();
        q.pop();
        if (max->getChildren().size() < (temp->getChildren().size())) {
            max = temp;
        }
        if (!temp->getChildren().empty()) {
            for (unsigned int i = 0; i < (temp->getChildren()).size(); ++i) {
                q.push(temp->getChildren()[i]);
            }
        }
    }
    return max->returnNode();
}

RootTree::RootTree(int rootLabel) : Tree(rootLabel) {}

int RootTree::traceTree() { return this->node; }