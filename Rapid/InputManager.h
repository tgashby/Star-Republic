#pragma once
#include "Player.h"

class InputManager{
public:

int w, a, s, d, dx, dy, prevX, prevY;
Player* player;

InputManager(Player* p);
~InputManager();
void keyCallBack(unsigned char key, int x, int y);
void keyUpCallBack(unsigned char key, int x, int y);
void mouseMotion(int x, int y);
};
