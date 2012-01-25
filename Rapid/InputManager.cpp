#include "InputManager.h"
#include "Player.h"

InputManager::InputManager(Player* p)
{
   this->player = p;
   SENSITIVITY = 80;
   AbsX = 0;
   AbsY = 0;
   dx = 0;
   dy = 0;
   prevX = 0;
   prevY = 0;
   glutWarpPointer(500, 300);
}

InputManager::~InputManager(){
}

void InputManager::keyCallBack(unsigned char key, int x, int y) {
   
   if (key == 'w')
	   w = 1;
   if (key == 's')
	   s = 1;
   if (key == 'a')
	   a = 1;
   if (key == 'd')
	   d = 1;
}

void InputManager::keyUpCallBack(unsigned char key, int x, int y) {
   if (key == 27)
	   exit(0);

   if (key == 'w')
	   w = 0;
   if (key == 's')
	   s = 0;
   if (key == 'a')
	   a = 0;
   if (key == 'd')
	   d = 0;
}

void InputManager::mouseMotion(int x, int y) {
   dx = prevX - x;
   dy = prevY - y;
   prevX = x;
   prevY = y;
   // assuming 500,500 is the glutWarpedPointer location
   AbsX = (x - 500) / SENSITIVITY;
   AbsY = (300 - y) / SENSITIVITY;
}
