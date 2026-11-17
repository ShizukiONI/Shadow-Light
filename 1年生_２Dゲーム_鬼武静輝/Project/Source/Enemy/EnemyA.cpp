#include "EnemyA.h"
#include "../Camera/Camera.h"

EnemyData g_Enemy[ENEMY_MAX];

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

void LoadEnemy()
{
	g_EnemyGraph = LoadGraph("Data/Enemy/Enemy_wallk_test.png");

	g_Enemy[0].animation[ENEMY_ANIM_RUN].handle = LoadGraph("Data/Enemy/Enemy_wallk_test.png");
	g_Enemy[0].animation[ENEMY_ANIM_RUN].invertedhandle = LoadGraph("Data/Enemy/Enemy_wallk_test.png");
	GraphFilter(g_Enemy[0].animation[ENEMY_ANIM_RUN].invertedhandle, DX_GRAPH_FILTER_INVERT);
}

void StartEnemy()
{
	StartEnemyAnimation;
}

void UpdateEnemy()
{
}

void DrawEnemy()
{
	CameraData camera = GetCamera();


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