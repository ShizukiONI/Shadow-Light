#pragma once
#include "DxLib.h"

//Player
bool CheckPlayerBlockCollision(VECTOR playerPos, int mapX, int mapY);
float CheckPlayerCollisionX(float nextX, float playerY);
float CheckPlayerCollisionY(float nextY, float playerX);
int CheckPlayerEnemyACollision();
bool CheckPlayerAttackEnemyCollision(int enemyIndex);

//EnemyA
bool CheckEnemyABlockCollision(VECTOR enemyApos, int mapX, int mapY);
float CheckEnemyACollisiomX(float nextX, float enemyAY, float currentX);
float CheckEnemyACollisionY(float nextY, float enamyAY, float currentY);
