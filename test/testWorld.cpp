#include <vector>
#include "../src/game/World.h"
#include "../src/game/WorldPoint.h"
#include <assert.h>
#include <iostream>

int main() {
  World world1 ("testWorld1.wf");

  WorldPoint current = world1.getCurrent();
  WorldPoint pnt0 = world1.getAt(0);
  WorldPoint pnt1 = world1.getAt(1);
  WorldPoint pnt2 = world1.getAt(2);
  WorldPoint pnt3 = world1.getAt(3);
  
  //cout << pnt2.getLeftID() << "\n";
  assert(pnt2.getLeftID() == 3);
  assert(pnt1.getLeftID() == 2);
  
  {
    assert(current.getNumberOfIDs() == 1);
    assert(current.getLeftID() == 1);
    
    world1.setChoice(current.getLeftID());
    current = world1.getCurrent();
    
    assert(current.getNumberOfIDs() == 1);
    assert(current.getLeftID() == 2);
  }
}
