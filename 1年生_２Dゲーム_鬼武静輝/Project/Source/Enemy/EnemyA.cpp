#include "EnemyA.h"
#include "../Camera/Camera.h"
#include "../Map/MapChip.h"

EnemyData g_Enemy[ENEMY_MAX];

int g_EnemyMapCount = 0;

int g_EnemyGraph;

void InitEnemy()
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		g_Enemy[i].active = false;
		g_Enemy[i].isTurn = false;
		g_Enemy[i].pos = VGet(0.0f, 0.0f, 0.0f);

		for (int j = 0; j < ENEMY_ANIM_MAX; j++)
		{
			InitAnimation(&g_Enemy[i].animation[j]);
		}

		g_Enemy[i].playAnim = ENEMY_ANIM_NONE;
	}
}

void LoadEnemyMap()
{
	FILE* fp;

	if (fopen_s(&fp, "Data/Map/LightMap.bin", "rb") != 0)return;

	g_EnemyMapCount = 0;

	for (int i = 0; i < MAP_CHIP_Y_NUM; i++)
	{
		for (int j = 0; j < MAP_CHIP_X_NUM; j++)
		{
			int map = fgetc(fp);

			if (map == 3 && g_EnemyMapCount < ENEMY_MAX)
			{
				g_Enemy[g_EnemyMapCount].pos = VGet(j * MAP_CHIP_WIDTH, i * MAP_CHIP_HEIGHT, 0.0f);

				g_Enemy[g_EnemyMapCount].active = true;

				g_EnemyMapCount++;
			}
		}
	}

	for(int i = 0; i < g_EnemyMapCount; i++)
	{
		g_Enemy[i].isTurn = false;
		g_Enemy[i].playAnim = ENEMY_ANIM_NONE;

		for (int j = 0; j < ENEMY_ANIM_MAX; j++)
		{
			InitAnimation(&g_Enemy[i].animation[j]);
		}
	}

	fclose(fp);
}

void LoadEnemy()
{
	g_EnemyGraph = LoadGraph("Data/Enemy/Enemy_wallk_test.png");

	g_Enemy[0].animation[ENEMY_ANIM_RUN].handle = LoadGraph("Data/Enemy/Enemy_wallk_test.png");
	g_Enemy[0].animation[ENEMY_ANIM_RUN].invertedhandle = LoadGraph("Data/Enemy/Enemy_wallk_test.png");
	GraphFilter(g_Enemy[0].animation[ENEMY_ANIM_RUN].invertedhandle, DX_GRAPH_FILTER_INVERT);

	LoadEnemyMap();
}

void StartEnemy()
{
	for (int i = 0; i < g_EnemyMapCount; i++)
	{
		StartEnemyAnimation(i, ENEMY_ANIM_RUN);
	}
}

void UpdateEnemy()
{
}

void DrawEnemy()
{
	if (!g_Enemy->active)
	{
		return;
	}

	CameraData camera = GetCamera();

	for (int i = 0; i < g_EnemyMapCount; i++)
	{
		EnemyData* enemy = &g_Enemy[i];

		if (!enemy->active)
		{
			continue;
		}

		EnemyAnimationType animType = g_Enemy->playAnim;
		AnimationData* animData = &g_Enemy->animation[animType];

		float drawX = g_Enemy->pos.x + (ENEMY_WIDTH - animData->frameWidth) / 2.0f;
	}


}

void FinEnemy()
{
}

void StartEnemyAnimation(int enemyIndex, EnemyAnimationType anim)
{
	EnemyData* enemy = &g_Enemy[enemyIndex];

	//再生中のアニメーションであれば何もしない
	if (anim == enemy->playAnim)return;

	//再生中アニメーション設定
	enemy->playAnim = anim;

	//再生中に必要なデータを取得
	AnimationData* animData = &enemy->animation[anim];

	//フレームをリセット
	animData->nowFrame = 0;
	animData->timer = 0;

	StartAnimation(animData, enemy->pos.x, enemy->pos.y, 8, 9, 128, 128,true);
}

void UpdateEnemyAnimation(int enemyIndex)
{
	EnemyData* enemy = &g_Enemy[enemyIndex];

	//歩行アニメーション再生
	StartEnemyAnimation(enemyIndex, ENEMY_ANIM_RUN);

	//アニメーション更新
	EnemyAnimationType animType = enemy->playAnim;

	if (animType != ENEMY_ANIM_NONE)
	{
		AnimationData* animData = &enemy->animation[animType];

		UpdateAnimation(animData);
	}
}