#pragma once

#define GHOSTCOUNT 4

struct MovingEnemy
{
	Vector2* position;
	Texture2D* texture;
	Rect* sourceRect;
	int direction;
	float speed;
};

MovingEnemy* _ghosts[GHOSTCOUNT];