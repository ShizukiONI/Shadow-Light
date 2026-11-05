#include "MapManager.h"
#include "MapParameter.h"
#include "MapChip.h"
#include "Block.h"
#include "../Player/Player.h"

// キャラクターの周囲何マスまでチェックするか
#define CHECK_ROUND_NUM (2)

void InitMap()
{
}

void LoadMap()
{
	LoadMapChipData(WORLD_LIGHT);
	LoadMapChipData(WORLD_SHADOW);
	LoadBlock();
}

void StartMap()
{
	CreateMap(WORLD_LIGHT);
	CreateMap(WORLD_SHADOW);
}

void DrawMap()
{
	// ブロック描画
	DrawBlock();
}

void FinMap()
{
}

