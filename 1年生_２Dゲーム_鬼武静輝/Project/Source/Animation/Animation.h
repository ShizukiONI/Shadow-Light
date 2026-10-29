#pragma once

struct AnimationData
{
	bool active;
	bool isLoop;
	int handle;
	int nowFrame;
	int frameNum;
	int frameWidth;
	int frameHeight;
	int timer;
	int interval;
	float posX;
	float posY;
};

void InitAnimation(AnimationData* animData);
void UpdateAnimation(AnimationData* animData);
void DrawAnimation(AnimationData* animData, bool isTurnX = false, bool isTurnY = false);
void DrawAnimation(AnimationData* animData, float posX, float posY,
	bool isTurnX = false, bool isTurnY = false);
void StartAnimation(AnimationData* animData, float posX, float posY,
	int interval, int frameNum, int width, int height, bool isLoop);
