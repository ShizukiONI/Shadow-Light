#pragma once
#include "DxLib.h"

//Player
bool CheckPlayerBlockCollision(VECTOR playerPos, int mapX, int mapY);
float CheckPlayerCollisionX(float nextX, float playerY);
float CheckPlayerCollisionY(float nextY, float playerX);

//EnemyA
bool CheckEnemyABlockCollsion(VECTOR enemyApos, int mapX, int mapY);
float CheckEnemyACollisiomX(float nexyX, float enemyAY);
float CheckEnemyACollisionY(float nextY, float enamyAY);
