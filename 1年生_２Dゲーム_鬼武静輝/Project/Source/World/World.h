#pragma once

enum WorldType
{
	WORLD_SHADOW,
	WORLD_LIGHT,
	WORLD_MAX
};

extern WorldType g_WorldType;
extern int g_WorldEffectTimer;
extern WorldType g_WorldBeforeEffect;
extern WorldType g_WorldAfterEffect;

extern int g_WorldBeforeScreen;
extern int g_WorldAfterScreen;
extern float g_WorldEffectX;

void InitWorld();
void ChangeWorld();
void UpdateWorld();
void UpdateWorldEffect();
void DrawWorldBackGround(WorldType world);
bool IsWorldEffectPassed(float scrrenX);