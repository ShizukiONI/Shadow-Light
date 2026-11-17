#pragma once
#include "DxLib.h"
#include "../Animation/Animation.h"

#define ENEMY_MAX (100)

enum  EnemyAnimationType
{
	ENEMY_ANIM_IDLE,
	ENEMY_ANIM_RUN,
	ENEMY_ANIM_MAX,
	ENEMY_ANIM_NONE = -1

};

struct  EnemyData
{
	bool active;
	VECTOR pos;
	bool isTurn;	//左を向いてるか
	AnimationData animation[ENEMY_ANIM_MAX];	//アニメーション管理
	EnemyAnimationType playAnim; //現在のアニメーション

};

extern EnemyData g_Enemy[ENEMY_MAX];

void InitEnemy();
void LoadEnemy();
void StartEnemy();
void UpdateEnemy();
void DrawEnemy();
void FinEnemy();
void StartEnemyAnimation(int enemyIndex, EnemyAnimationType anim);