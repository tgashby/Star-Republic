#pragma once

/**
 * State is an interface for game states
 * Each state must implement initialize, display, and cleanUp
 */
struct State
{
   State() { m_initialized = false; }
   
   /**
    * Initializes anything needed for the given state
    * This could be meshes, textures, etc
    */
   virtual void initialize() = 0;
   
   /**
    * Displays the State
    * Initialize must be called before this
    */
   virtual void display() = 0;
   
   /**
    * Cleans up the state, deallocating things, removing things, etc
    */
   virtual void cleanUp() = 0;
   
protected:
   bool m_initialized;
};