#include "MapChip.h"
#include "DxLib.h"
//#include "../GameSetting/GameSetting.h"
#include "MapParameter.h"
#include "Block.h"

MapChipData g_MapChip[WORLD_MAX][MAP_CHIP_Y_NUM][MAP_CHIP_X_NUM] = { 0 };


void LoadMapChipData(WorldType world)
{
	FILE* fp;
	const char* filePath;

	if (world == WORLD_LIGHT)
	{
		filePath = "Data/Map/LightMap.bin";
	}
	else
	{
		filePath = "Data/Map/ShadowMap.bin";
	}

	if (fopen_s(&fp, filePath, "rb") != 0)return;

	for (int i = 0; i < MAP_CHIP_Y_NUM; i++)
	{
		for (int j = 0; j < MAP_CHIP_X_NUM; j++)
		{
			int map = fgetc(fp);
			g_MapChip[world][i][j].mapChip = map;
		}
	}

	fclose(fp);
}

void CreateMap(WorldType world)
{
	for (int i = 0; i < MAP_CHIP_Y_NUM; i++)
	{
		for (int j = 0; j < MAP_CHIP_X_NUM; j++)
		{
			MapChipType type = (MapChipType)g_MapChip[world][i][j].mapChip;

			if (type == MAP_CHIP_NONE)continue;

			VECTOR pos = VGet(
				j * MAP_CHIP_WIDTH,
				i * MAP_CHIP_HEIGHT,
				0.0f
			);

			if (type == NORMAL_BLOCK)
			{
				g_MapChip[world][i][j].data = CreateBlock(NORMAL_BLOCK, pos);
			}
			else if (type == SHADOW_BLOCK)
			{
				g_MapChip[world][i][j].data = CreateBlock(SHADOW_BLOCK, pos);
			}
		}
	}
}

MapChipData GetMapChipData(int x, int y,WorldType world)
{
	return g_MapChip[world][y][x];
}
