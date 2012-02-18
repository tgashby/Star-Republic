#include "StateManager.h"

StateManager::StateManager()
{
}
	
	
StateManager::~StateManager()
{
   for (std::list<State*>::size_type i = 0; 
        i < m_states.size(); i++) 
   {
      popState();
   }
   
   m_states.clear();
}

void StateManager::pushState(State *newState)
{
   m_states.push_front(newState);
   
   newState->initialize();
}

void StateManager::popState()
{
   getCurrentState()->cleanUp();
   
   State* tempState = m_states.front();
   
   delete tempState;
   
   m_states.pop_front();
}

State* StateManager::getCurrentState()
{
   return m_states.front();
}