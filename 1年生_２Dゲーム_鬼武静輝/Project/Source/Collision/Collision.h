#pragma once
#include "DxLib.h"

bool CheckPlayerBlockCollision(VECTOR playerPos, int mapX, int mapY);

float CheckPlayerCollisionX(float nextX, float playerY);

float CheckPlayerCollisionY(float nextY, float playerX);
