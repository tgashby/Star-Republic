#include "testWorld.h"

void testLoop1()
{
  World world1 ("testWorld1.wf");

  WorldPoint current = world1.getCurrent();
  
  for (int i = 2; i < 30; i++) {
    current = world1.getCurrent();
    assert(current.getNumberOfIDs() == 1);
    assert(current.getFirstID() == i % 5);
    world1.setChoice(current.getFirstID());
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
	assert(current.getFirstID() == 5);
	assert(current.getSecondID() == 7);
	assert(current.getThirdID() == 10);

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
	assert(current.getFirstID() == 0);
	currentIndex = 0;
	world2.setChoice(currentIndex);
      }
      else if (currentIndex == 9) {
	assert(current.getFirstID() == 0);
	currentIndex = 0;
	world2.setChoice(currentIndex);
      }
      else if (currentIndex == 12) {
	assert(current.getFirstID() == 0);
	currentIndex = 0;
	world2.setChoice(currentIndex);
      }
      else {
	currentIndex++;
	assert(current.getFirstID() == currentIndex);
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
