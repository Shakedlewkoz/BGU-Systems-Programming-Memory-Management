#ifndef AGENT_H_
#define AGENT_H_

#include <vector>


using namespace std;

class Session;
class Agent {
public:
    Agent();
    virtual ~Agent() {};
    virtual Agent *clone() const = 0;
    virtual void act(Session &session) = 0;
};

class ContactTracer : public Agent {
public:
    ContactTracer();
    virtual ~ContactTracer() {};

    virtual void act(Session &session);

    virtual Agent *clone() const;

};


class Virus : public Agent {
public:
    Virus(int nodeInd);

    virtual ~Virus() {};

    virtual void act(Session &session);

    virtual Agent *clone() const;

private:
    const int nodeInd;
};

#endif
