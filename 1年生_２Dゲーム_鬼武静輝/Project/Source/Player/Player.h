#pragma once
#include "DxLib.h"
#include "../Animation/Animation.h"

//プレイヤーの実際のサイズ
#define PLAYER_WIDTH (65.0f)
#define PLAYER_HEIGHT (60.0f)

// プレイヤーの当たり判定サイズ
#define PLAYER_HIT_WIDTH  (30.0f)
#define PLAYER_HIT_HEIGHT (48.0f)

enum PlayerAnimationType
{

	PLAYER_ANIM_IDLE,
	PLAYER_ANIM_RUN,
	PLAYER_ANIM_JUMP,
	PLAYER_ANIM_FALL,
	PLAYER_ANIM_LANDING,
	PLAYER_ANIM_ATTACK,
	PLAYER_ANIM_MAX,
	PLAYER_ANIM_NONE = -1
};


struct PlayerData 
{
	bool active;	//有効フラグ
	bool isTurn;	//左を向いているか
	bool isAir; //空中にいるかどうか
	VECTOR pos;	//プレイヤー座標
	float moveX;	//X移動量
	float moveY;	//Y移動量
	float speed;	//移動速度

	//攻撃
	bool isAttack;
	bool isAttackHit;
	int attackTimer;
	int attackcoolTimer;

	// アニメーション管理
	AnimationData animation[PLAYER_ANIM_MAX];
	PlayerAnimationType playAnim;

};

//関数の宣言
void InitPlayer();
void LoadPlayer();
void StartPlayer();
void StepPlayer();
void UpdatePlayer();
void DrawPlayer();
void FinPlayer();

PlayerData GetPlayer();