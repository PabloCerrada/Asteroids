#pragma once
#include "Manager.h"
#include <stack>


class GameStateMachine : public Singleton<GameStateMachine>
{
private:
    stack<Manager*> stackStates;
    stack<Manager*> stackToDelete; // pila de escenas a borrar
public:
    GameStateMachine() {};
    ~GameStateMachine();
    Manager* currentState();
    void clearState();
    void pushState(Manager* state);
    void changeState(Manager* state);
    void popState();
    int stackLength();
};

