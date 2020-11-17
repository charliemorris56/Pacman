#pragma once

//#define MUNCHIECOUNT 50

//Structure Definition
struct Enemy
{
	int currentFrameTime;
	int frameTime;
	int frame;
	Rect* sourceRect;
	Texture2D* texture;
	Vector2* position;
};

Enemy* _munchie;
Enemy* _cherry;