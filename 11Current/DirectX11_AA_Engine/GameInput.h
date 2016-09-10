#ifndef GAME_INPUT_H
#define GAME_INPUT_H

#include "constants.h"

struct InputButton
{
	bool isDown;
};

struct GameInput
{
	InputButton moveRight;
	InputButton moveLeft;
	InputButton moveUp;
	InputButton moveDown;

	InputButton actionRight;
	InputButton actionLeft;
	InputButton actionUp;
	InputButton actionDown;

	InputButton rightBumper;
	InputButton leftBumper;

	InputButton back;
	InputButton start;
};

#endif