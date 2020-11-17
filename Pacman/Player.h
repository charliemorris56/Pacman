#pragma once

//Structure Definition
struct Player
{
	float speedMultiplier;
	float speed;
	int currentFrameTime;
	int frameTime;
	int direction;
	int frame;
	int score;
	bool dead;
	bool win;
	Rect* sourceRect;
	Texture2D* texture;
	Vector2* position;
};

Player* _pacman;