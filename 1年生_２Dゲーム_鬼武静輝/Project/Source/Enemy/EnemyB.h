#pragma once
#include "DxLib.h"
#include "../Animation/Animation.h"

#define ENEMY_B_MAX (100)
#define ENEMY_B_MAP_MAX (100)
#define ENEMY_B_MAX_HP (2)
#define ENEMY_B_SPEED (2.0f)
#define ENEMY_B_WIDTH (100)
#define ENEMY_B_HEIGHT (100)
#define ENEMY_B_HIT_WIDTH (30)
#define ENEMY_B_HIT_HEIGHT (30)
#define ENEMY_B_GRAVITY (0.5f)

enum EnemyBAnimationType
{
	ENEMY_B_ANIM_IDLE,
	ENEMY_B_ANIM_RUN,
	ENEMY_B_ANIM_MAX,
	ENEMY_B_ANIM_NONE = -1
};

struct  EnemyBdata
{
	int hp;
	int hitStopTimer;
	bool isTurn;
	bool active;
	VECTOR pos;
	float velocityX;
	float velocityY;

	AnimationData animation[ENEMY_B_ANIM_MAX];
	EnemyBAnimationType playAnim;

};

extern EnemyBdata g_EnemyB[ENEMY_B_MAX];
extern int g_EnemyBMapCount;

void InitEnemyB();
void LoadEnemyB();
void LoadEnemyBMap();
void StartEnemyB();
void UpdateEnemyB();
void DrawEnemyB();
void FinEnemyB();
void StartEnemyBanimation(int enemyIndex, EnemyBAnimationType anim);