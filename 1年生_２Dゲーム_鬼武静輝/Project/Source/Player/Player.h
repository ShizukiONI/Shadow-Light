#pragma once
#include "DxLib.h"

#define PLAYER_WIDTH (40.0f)
#define PLAYER_HEIGHT (60.0f)


struct PlayerData 
{
	bool active;	//有効フラグ
	bool isTurn;	//左を向いているか
	float posX;		//X座標
	float posY;		//Y座標
	float moveX;	//X移動量
	float moveY;	//Y移動量
	float speed;	//移動速度

	bool isAir; //空中にいるかどうか

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