#include "Block.h"
#include "../World/World.h"
//#include "../Camera/Camera.h"

BlockData g_Blocks[BLOCK_MAX] = { 0 };
int g_BlockHandle[BLOCK_TYPE_MAX] = { 0 };


void InitBlock()
{
	BlockData* block = g_Blocks;
	for (int i = 0; i < BLOCK_MAX; i++, block++)
	{
		block->active = false;
		block->pos = VGet(0.0f, 0.0f, 0.0f);
		block->world = WORLD_LIGHT;
	}
}

void LoadBlock()
{
	g_BlockHandle[NORMAL_BLOCK] = LoadGraph("Data/Map/MapBlock.png");
	g_BlockHandle[SHADOW_BLOCK] = LoadGraph("Data/Map/MapBlock.png");
}

void StartBlock()
{
}

void StepBlock()
{
}

void DrawBlock()
{
	BlockData* block = g_Blocks;

	for (int i = 0; i < BLOCK_MAX; i++, block++)
	{
		if (block->active && block->world == g_WorldType)
		{
			if (block->type == NORMAL_BLOCK)
			{
				// 光の世界：黒いブロック
				SetDrawBright(0, 0, 0);

				DrawGraph(
					(int)block->pos.x,
					(int)block->pos.y,
					block->handle,
					TRUE
				);

				// 明るさを元に戻す
				SetDrawBright(255, 255, 255);
			}
			else if (block->type == SHADOW_BLOCK)
			{
				// 影の世界：白いブロック
				DrawGraph(
					(int)block->pos.x,
					(int)block->pos.y,
					block->handle,
					TRUE
				);
			}
		}
	}
}

void FinBlock()
{
}

BlockData* CreateBlock(MapChipType type, VECTOR pos)
{
	// 未使用のブロックを探す
	BlockData* block = g_Blocks;
	for (int i = 0; i < BLOCK_MAX; i++, block++)
	{
		if (!block->active)
		{
			// ブロック生成
			block->active = true;
			block->handle = g_BlockHandle[type];
			block->pos = pos;
			block->type = type;

			if (type == NORMAL_BLOCK)
			{
				block->world = WORLD_LIGHT;
			}
			else if (type == SHADOW_BLOCK)
			{
				block->world = WORLD_SHADOW;
			}

			return block;
		}
	}

	return nullptr;
}

BlockData* GetBlocks()
{
	return g_Blocks;
}
