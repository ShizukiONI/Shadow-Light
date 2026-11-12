#pragma once

enum WorldType
{
	WORLD_SHADOW,
	WORLD_LIGHT,
	WORLD_MAX
};

extern WorldType g_WorldType;

void InitWorld();
void ChangeWorld();
void UpdateWorld();
void UpdateWorldEffect();
void DrawWorldEffect();