#include "InputManager.h"

InputManager::InputManager(Player p)
{
   this->player = p;
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
   int dx = prevX - x;
   int dy = prevY - y;
}
