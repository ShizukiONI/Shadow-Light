 #include "Block.h"
#include "../World/World.h"
#include "../Camera/Camera.h"

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

    CameraData camera = GetCamera();

    for (int i = 0; i < BLOCK_MAX; i++, block++)
    {
        if (block->active)
        {
            bool isCurrentWorld = (block->world == g_WorldType);

            int drawX = (int)((block->pos.x - camera.pos.x) * CAMERA_ZOOM);
            int drawY = (int)((block->pos.y - camera.pos.y) * CAMERA_ZOOM);

            int width = (int)(MAP_CHIP_WIDTH * CAMERA_ZOOM);
            int height = (int)(MAP_CHIP_HEIGHT * CAMERA_ZOOM);

            if (isCurrentWorld)
            {
                if (block->type == NORMAL_BLOCK)
                {
                    // 光の世界：黒いブロック
                    SetDrawBright(0, 0, 0);

                    DrawExtendGraph(
                        drawX,
                        drawY,
                        drawX + width,
                        drawY + height,
                        block->handle,
                        TRUE
                    );

                    SetDrawBright(255, 255, 255);
                }
                else if (block->type == SHADOW_BLOCK)
                {
                    // 影の世界：白いブロック
                    DrawExtendGraph(
                        drawX,
                        drawY,
                        drawX + width,
                        drawY + height,
                        block->handle,
                        TRUE
                    );
                }
            }
            else
            {
                // 反対世界：薄い輪郭だけ表示
                int outlineColor;

                if (block->type == NORMAL_BLOCK)
                {
                    // 黒いブロックには薄い白色の輪郭
                    outlineColor = GetColor(255, 255, 255);
                }
                else
                {
                    // 白いブロックには薄い黒色の輪郭
                    outlineColor = GetColor(0, 0, 0);
                }

                SetDrawBlendMode(DX_BLENDMODE_ALPHA, 13);

                DrawBox(
                    drawX,
                    drawY,
                    drawX + width,
                    drawY + height,
                    outlineColor,
                    FALSE
                );

                // 透明度をリセット
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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
