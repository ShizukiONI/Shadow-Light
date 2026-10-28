#pragma once
#include "DxLib.h"

#define BLOCK_MAX 1024
#define MAP_CHIP_X_NUM (64)
#define MAP_CHIP_Y_NUM (18)
#define MAP_CHIP_WIDTH (20.0f)
#define MAP_CHIP_HEIGHT (20.0f)

enum MapChipType
{
	MAP_CHIP_NONE,
	NORMAL_BLOCK,
	BLOCK_TYPE_MAX,
};

struct BlockData
{
	bool active;
	int handle;
	MapChipType type;
	VECTOR pos;
};

struct MapChipData
{
	int mapChip;
	BlockData* data;
};
