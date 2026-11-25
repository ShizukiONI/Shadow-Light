#pragma once
#include "DxLib.h"
#include "../Animation/Animation.h"

// 上限数
#define ENEMY_MAX          (100)
#define ENEMY_MAP_MAX      (100)

// 基本ステータス
#define ENEMY_MAX_HP       (3)
#define ENEMY_SPEED        (2.0f)

// 描画サイズ・当たり判定
#define ENEMY_WIDTH        (100)
#define ENEMY_HEIGHT       (100)
#define ENEMY_HIT_WIDTH    (38)
#define ENEMY_HIT_HEIGHT   (38)

// 物理
#define ENEMY_GRAVITY      (0.5f)

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
	float velocityX;
	float velocityY;
	
	int hitStopTimer;
	int hp;
	bool isTurn;	//左を向いてるか

	AnimationData animation[ENEMY_ANIM_MAX];	//アニメーション管理
	EnemyAnimationType playAnim;	//現在のアニメーション

};

extern EnemyData g_Enemy[ENEMY_MAX];
extern int g_EnemyMapCount;

void InitEnemy();
void LoadEnemy();
void LoadEnemyMap();
void StartEnemy();
void UpdateEnemy();
void DrawEnemy();
void FinEnemy();
void StartEnemyAnimation(int enemyIndex, EnemyAnimationType anim);