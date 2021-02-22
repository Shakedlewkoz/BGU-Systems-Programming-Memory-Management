#ifndef TREE_H_
#define TREE_H_

#include <vector>
#include "Graph.h"
#include <list> //meyutar?

using namespace std;

class Session;
class Tree{
public:
    Tree(int rootLabel);
    //Rule of 5:
    virtual ~Tree();
    void clone(const Tree &other);
    Tree(const Tree &other); //copy constructor
    Tree & operator=(const Tree &other); //copy assignment operator
    Tree(Tree &&other); //move constructor
    const Tree& operator=(Tree &&other); //move assignment operator
    static Tree* createTree(const Session& session, int rootLabel);
    void addChild(const Tree& child);
    void addChildHelper(Tree* child);
    static Tree* bfs(const Session &session, int rootLabel);
    virtual int traceTree()=0;
    vector<Tree*> getChildren() const;
    void clear();
    virtual int returnNode();

protected:
    int node;
    vector<Tree*> children;
};

class CycleTree: public Tree{
public:
    CycleTree(int rootLabel, int currCycle);
    virtual int traceTree();
private:
    int currCycle;
};

class MaxRankTree: public Tree{
public:
    MaxRankTree(int rootLabel);
    virtual int traceTree();
};

class RootTree: public Tree{
public:
    RootTree(int rootLabel);
    virtual int traceTree();
};

#endif
