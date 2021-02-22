#include "../include/Agent.h"
#include "../include/Tree.h"
#include "../include/Session.h"
#include <iostream>

/* ---------- Agent ---------- */

Agent::Agent() = default;

/* ---------- ContactTracer ---------- */

/**
 * default constructor for CT
 */

ContactTracer::ContactTracer() = default;

/**
 * copy constructor for CT
 * @return a new CT object.
 */
Agent *ContactTracer::clone() const {
    return new ContactTracer(*this);
}


void ContactTracer::act(Session &session) {
    /* dequeues an infected node from the infected nodes que and creates a tree for it
     * and then gets a node using trace tree and isolates it from the graph.
     * */

    /* The contact tracer will act only if the infected que is empty */
    if (!session.infectedQueEmpty()) {



        Tree * pT = Tree::bfs(session, session.dequeueInfected());
        int nodeToIsolate = pT->traceTree();
        pT->clear();
        delete pT;
        pT = nullptr;
        session.contactTracerAct(nodeToIsolate);

    }
}

/* ---------- Virus ---------- */

/**
 * constructor for a Virus
 * @param nodeInd is the infected node.
 */
Virus::Virus(int nodeInd) : nodeInd(nodeInd) {
}

/**
 * copy constructor for Virus
 * @return
 */
Agent *Virus::clone() const {
    return new Virus(*this);
}

/**
 * Act method for virus.
 * @param session
 */
void Virus::act(Session &session) {
    /* made an Act method in session for easier access to graph*/
    session.virusAct(this->nodeInd);
    return;
}

