#include "DxLib.h"
#include "World.h"
#include "../Input/Input.h"

WorldType g_WorldType = WORLD_LIGHT;

//初期化
void InitWorld()
{
	g_WorldType = WORLD_LIGHT;
}

//世界の切り替え
void ChangeWorld()
{
	if (g_WorldType == WORLD_LIGHT)
	{
		g_WorldType = WORLD_SHADOW;
	}
	else
	{
		g_WorldType = WORLD_LIGHT;
	}
}

void UpdateWorld()
{
	if (IsTriggerKey(KEY_INPUT_X))
	{
		ChangeWorld();
	}
}
