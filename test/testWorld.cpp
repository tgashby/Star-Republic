#include "testWorld.h"

void testLoop1()
{
  World world1 ("testWorld1.wf");

  WorldPoint current = world1.getCurrent();
  
  for (int i = 1; i < 30; i++) {
    current = world1.getCurrent();
    assert(current.getNumberOfIDs() == 1);
    assert(current.getLeftID() == i % 5);
    world1.setChoice(current.getLeftID());
  }
}

void testPath1()
{
  World world2 ("testWorld2.wf");
  WorldPoint current = world2.getCurrent();
  int currentIndex = 0;
  bool checkLeft = false;
  bool checkMid = false;
  bool checkRight = false;

  for (int i = 0; i < 100; i++)
    {
      current = world2.getCurrent();
      if (currentIndex == 4) {
	assert(current.getLeftID() == 5);
	assert(current.getMiddleID() == 7);
	assert(current.getRightID() == 10);

	if (!checkLeft) {
	  checkLeft = true;
	  currentIndex = 5;
	  world2.setChoice(currentIndex);
	}
	else if (!checkMid) {
	  checkMid = true;
	  currentIndex = 7;
	  world2.setChoice(currentIndex);
	}
	else if (!checkRight) {
	  checkRight = true;
	  currentIndex = 10;
	  world2.setChoice(currentIndex);
	}
	else {
	  currentIndex = 10;
	  world2.setChoice(currentIndex);
	}
      }
      else if (currentIndex == 6) {
	assert(current.getLeftID() == 0);
	currentIndex = 0;
	world2.setChoice(currentIndex);
      }
      else if (currentIndex == 9) {
	assert(current.getLeftID() == 0);
	currentIndex = 0;
	world2.setChoice(currentIndex);
      }
      else if (currentIndex == 12) {
	assert(current.getLeftID() == 0);
	currentIndex = 0;
	world2.setChoice(currentIndex);
      }
      else {
	currentIndex++;
	assert(current.getLeftID() == currentIndex);
	world2.setChoice(currentIndex);
      }
      
    }
  assert(checkLeft);
  assert(checkMid);
  assert(checkRight);
}

int main() {
  testLoop1();
  testPath1();
}
