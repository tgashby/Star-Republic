#include "InputManager.h"
#include "Player.h"
#include <stdio.h>

InputManager::InputManager(Player* p)
{
   this->player = p;
   p->setRefx(500);
   p->setRefy(300);
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
   if (key == 'a'){
	   a = 1;
   }
   if (key == 'd')
	   d = 1;
   if (key == 'z'){
           player->setFiring(true);
           printf("\nZ Button Was Pressed!\n");
   }
   if (key == ' '){
           player->speedUp();
           printf("\nSPACE Button Was Pressed!\n");
   }
}

void InputManager::keyUpCallBack(unsigned char key, int x, int y) {
   if (key == 27)
	   exit(0);

   if (key == 'w')
	   w = 0;
   if (key == 's')
	   s = 0;
   if (key == 'a'){
	   a = 0;
   }
   if (key == 'd')
	   d = 0;
   if (key == 'z'){
           player->setFiring(false);
           printf("\nZ Button Was Released!\n");
   }
   if (key == ' '){
           player->slowDown();
           printf("\nSPACE Button Was Released!\n");
   }
}

void InputManager::mouseMotion(int x, int y) {
   dx = prevX - x;
   dy = prevY - y;
   prevX = x;
   prevY = y;
   // assuming 500,500 is the glutWarpedPointer location
   AbsX = (x - 500) / SENSITIVITY;
   AbsY = (300 - y) / SENSITIVITY;
   player->setRefx(AbsX);
   player->setRefy(AbsY);
}
