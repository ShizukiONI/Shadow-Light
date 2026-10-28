#include "MapManager.h"
#include "MapParameter.h"
#include "MapChip.h"
#include "Block.h"
//#include "../Player/Player.h"

// キャラクターの周囲何マスまでチェックするか
#define CHECK_ROUND_NUM (2)

void InitMap()
{
}

void LoadMap()
{
	LoadMapChipData();
	LoadBlock();
}

void StartMap()
{
	CreateMap();
}

void DrawMap()
{
	// ブロック描画
	DrawBlock();
}

void FinMap()
{
}

//void ChceckMapPlayerCollision()
//{
//	PlayerData player = GetPlayer();
//	// プレイヤーの足元の座標をマップチップのインデックスに変換
//	int playerX = (int)(player.posX / MAP_CHIP_WIDTH);
//	int playerY = (int)((player.posY + PLAYER_HEIGHT) / MAP_CHIP_HEIGHT);
//	// プレイヤーの左上にあるマップチップインデックス
//	int left = playerX - CHECK_ROUND_NUM;
//	int top = playerY - CHECK_ROUND_NUM;
//	// プレイヤーの右下にあるマップチップインデックス
//	int right = playerX + CHECK_ROUND_NUM;
//	int bottom = playerY + CHECK_ROUND_NUM;
//
//	// まずはX軸でチェックする
//
//	// 左上から3マスずつチェックしていく
//	for (int y = top; y <= bottom; y++)
//	{
//		// マップチップからはみ出したら処理しなくていい
//		if (y < 0 || y >= MAP_CHIP_Y_NUM) continue;
//
//		for (int x = left; x <= right; x++)
//		{
//			// マップチップからはみ出したら処理しなくていい
//			if (x < 0 || x >= MAP_CHIP_X_NUM) continue;
//
//			// マップチップ番号を取得
//			MapChipData mapChipData = GetMapChipData(x, y);
//			switch (mapChipData.mapChip)
//			{
//			case NORMAL_BLOCK:
//				PlayerHitNormalBlockX(mapChipData);
//				break;
//			}
//		}
//	}
//
//	// X軸チェック後にY軸チェックする
//
//	// 左上から3マスずつチェックしていく
//	for (int y = top; y <= bottom; y++)
//	{
//		// マップチップからはみ出したら処理しなくていい
//		if (y < 0 || y >= MAP_CHIP_Y_NUM) continue;
//
//		for (int x = left; x <= right; x++)
//		{
//			// マップチップからはみ出したら処理しなくていい
//			if (x < 0 || x >= MAP_CHIP_X_NUM) continue;
//
//			// マップチップ番号を取得
//			MapChipData mapChipData = GetMapChipData(x, y);
//			switch (mapChipData.mapChip)
//			{
//			case NORMAL_BLOCK:
//				PlayerHitNormalBlockY(mapChipData);
//				break;
//			}
//		}
//	}
//
//}
