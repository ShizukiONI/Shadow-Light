#include "Block.h"
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
		block->type = MAP_CHIP_NONE;
	}
}

void LoadBlock()
{
	g_BlockHandle[NORMAL_BLOCK] = LoadGraph("Data/Map/MapBlock.png");
}

void StartBlock()
{
}

void StepBlock()
{
}

void DrawBlock()
{
	//// スクロールのためカメラを取得
	//CameraData camera = GetCamera();

	BlockData* block = g_Blocks;
	for (int i = 0; i < BLOCK_MAX; i++, block++)
	{
		if (block->active)
		{
			DrawGraph((int)(block->pos.x), (int)(block->pos.y), block->handle, TRUE);
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
			return block;
		}
	}

	return nullptr;
}

BlockData* GetBlocks()
{
	return g_Blocks;
}
