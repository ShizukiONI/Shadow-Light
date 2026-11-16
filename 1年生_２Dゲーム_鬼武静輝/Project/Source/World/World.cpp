#include "DxLib.h"
#include "World.h"
#include "../Input/Input.h"

WorldType g_WorldType = WORLD_LIGHT;

WorldType g_WorldBeforeEffect = WORLD_LIGHT;
WorldType g_WorldAfterEffect = WORLD_SHADOW;

int g_WorldEffectTimer = 0;

#define WORLD_EFFECT_DURATION (15)
#define WORLD_EFFECT_WIDTH (1280.0f)
#define WORLD_EFFECT_HEIGHT (720.0f)

float g_WorldEffectX = 0.0f;

int g_WorldBeforeScreen = -1;
int g_WorldAfterScreen = -1;

//初期化
void InitWorld()
{
	g_WorldType = WORLD_LIGHT;

	g_WorldBeforeScreen = MakeScreen(1280, 720, TRUE);
	g_WorldAfterScreen = MakeScreen(1280, 720, TRUE);
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
	if (g_WorldEffectTimer == 0)
	{
		if (IsTriggerKey(KEY_INPUT_X))
		{
			g_WorldBeforeEffect = g_WorldType;

			if (g_WorldType == WORLD_LIGHT)
			{
				g_WorldAfterEffect = WORLD_SHADOW;
			}
			else
			{
				g_WorldAfterEffect = WORLD_LIGHT;
			}

			g_WorldEffectTimer = 1;
			g_WorldEffectX = -WORLD_EFFECT_WIDTH;
		}
	}
}

void UpdateWorldEffect()
{
	if (g_WorldEffectTimer > 0)
	{
		g_WorldEffectTimer++;

		//移動位置を更新
		g_WorldEffectX = -WORLD_EFFECT_WIDTH + (WORLD_EFFECT_WIDTH + 2.0f) * ((float)g_WorldEffectTimer / WORLD_EFFECT_DURATION);

		//演出終了
		if (g_WorldEffectTimer >= WORLD_EFFECT_DURATION)
		{
			g_WorldType = g_WorldAfterEffect;
			g_WorldEffectTimer = 0;
		}
	}
}

void DrawWorldBackGround(WorldType world)
{
	if (world == WORLD_LIGHT)
	{
		DrawBox(0, 0, 1280, 720,
			GetColor(255, 255, 255), TRUE);
	}
	else
	{
		DrawBox(0, 0, 1280, 720,
			GetColor(0, 0, 0), TRUE);
	}
}


bool IsWorldEffectPassed(float scrrenX)
{
	if (g_WorldEffectTimer <= 0)
	{
		return false;
	}

	return scrrenX < g_WorldEffectX + WORLD_EFFECT_WIDTH;
}
