#pragma once

#include <list>
#include "State.h"

/**
 * StateManager handles the different states of the game.
 * Ex: Menu, Options, Pause, Game, Win, Lose
 * Keeps a stack of states that are pushed and popped
 * Expects States to be allocated before being pushed
 * Deallocates the States when it pops them
 */
class StateManager
{
public:
   /**
    * Constructor for StateManager, doesn't do anything.
    */
	StateManager();
   
   /**
    * Destructor for StateManager, frees the memory of each state,
    * clears the list of states
    */
	~StateManager();
   
   /**
    * Push a new state onto the stack, making it the current one
    * @param newState the new state to push onto the stack
    */
   void pushState(State* newState);
   
   /**
    * Pop the current state from the stack, making the previous state the
    * current one
    */
   void popState();
   
   /**
    * Return the current state, so that we can draw it, update it, etc
    */
   State* getCurrentState();
   
private:
   std::list<State*> m_states;
};
