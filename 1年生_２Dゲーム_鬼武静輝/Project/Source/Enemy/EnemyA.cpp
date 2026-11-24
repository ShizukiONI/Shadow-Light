#include "EnemyA.h"
#include "../Camera/Camera.h"
#include "../Map/MapChip.h"
#include "../Collision/Collision.h"

EnemyData g_Enemy[ENEMY_MAX];

int g_EnemyMapCount = 0;

int g_EnemyGraph;

void UpdateEnemyAnimation(int enemyIndex);

void InitEnemy()
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		g_Enemy[i].active = false;
		g_Enemy[i].isTurn = false;
		g_Enemy[i].pos = VGet(0.0f, 0.0f, 0.0f);
		g_Enemy[i].velocityX = 0.0f;
		g_Enemy[i].velocityY = 0.0f;
		g_Enemy[i].hitStopTimer = 0.0;
		g_Enemy[i].hp = ENEMY_MAX_HP;

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
				g_Enemy[g_EnemyMapCount].hp = ENEMY_MAX_HP;
				g_Enemy[g_EnemyMapCount].hitStopTimer = 0;

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
	LoadEnemyMap();

	g_EnemyGraph = LoadGraph("Data/Enemy/Enemy_wallk_test.png");

	g_Enemy[0].animation[ENEMY_ANIM_RUN].handle = LoadGraph("Data/Enemy/Enemy_wallk_test.png");
	g_Enemy[0].animation[ENEMY_ANIM_RUN].invertedhandle = LoadGraph("Data/Enemy/Enemy_wallk_test.png");
	GraphFilter(g_Enemy[0].animation[ENEMY_ANIM_RUN].invertedhandle, DX_GRAPH_FILTER_INVERT);

	g_Enemy[0].animation[ENEMY_ANIM_IDLE].handle = LoadGraph("Data/Enemy/Enemy_idle.png");
	g_Enemy[0].animation[ENEMY_ANIM_IDLE].invertedhandle = LoadGraph("Data/Enemy/Enemy_idle.ong");
	GraphFilter(g_Enemy[0].animation[ENEMY_ANIM_IDLE].invertedhandle, DX_GRAPH_FILTER_INVERT);
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
	for (int i = 0; i < g_EnemyMapCount; i++)
	{
		if (!g_Enemy[i].active)
		{
			continue;
		}

		EnemyData* enemy = &g_Enemy[i];

		// 重力を加算
		enemy->velocityY += ENEMY_GRAVITY;

		// 接触による停止処理
		if (enemy->hitStopTimer > 0)
		{
			enemy->hitStopTimer--;
			continue;
		}

		//移動速度
		enemy->velocityX = enemy->isTurn ? -ENEMY_SPEED : ENEMY_SPEED;

		// 画像の描画位置を計算
		float drawX = enemy->pos.x
			+ (MAP_CHIP_WIDTH - ENEMY_WIDTH) / 2.0f;

		float drawY = enemy->pos.y
			+ (MAP_CHIP_HEIGHT - ENEMY_HEIGHT) / 2.0f;

		float nextX = drawX + enemy->velocityX;

		float resultX = CheckEnemyACollisiomX(nextX, drawY, drawX);

		enemy->pos.x = resultX - (MAP_CHIP_WIDTH - ENEMY_WIDTH) / 2.0f;

		// 次のY座標を計算
		float nextY = drawY + enemy->velocityY;

		if (resultX != nextX)
		{
			enemy->isTurn = !enemy->isTurn;
		}

		// 床・天井との衝突判定
		float resultY = CheckEnemyACollisionY(
			nextY, drawX, drawY);

		// 座標を更新
		enemy->pos.y = resultY
			- (MAP_CHIP_HEIGHT - ENEMY_HEIGHT) / 2.0f;

		// 衝突したら落下速度をリセット
		if (resultY != nextY)
		{
			enemy->velocityY = 0.0f;
		}

		// アニメーション更新
		UpdateEnemyAnimation(i);
	}
}

void DrawEnemy()
{
	CameraData camera = GetCamera();

	for (int i = 0; i < g_EnemyMapCount; i++)
	{
		EnemyData* enemy = &g_Enemy[i];

		if (!enemy->active)
		{
			continue;
		}

		EnemyAnimationType animType = enemy->playAnim;
		AnimationData* animData = &g_Enemy->animation[animType];

		float drawX = enemy->pos.x + (MAP_CHIP_WIDTH - ENEMY_WIDTH) / 2.0f;
		float drawY = enemy->pos.y +(MAP_CHIP_HEIGHT - ENEMY_HEIGHT) / 2.0f;

		DrawAnimation(animData, (drawX - camera.pos.x) * CAMERA_ZOOM,
								(drawY - camera.pos.y) * CAMERA_ZOOM,
			enemy->isTurn, false, CAMERA_ZOOM * (ENEMY_WIDTH / 128.0f),CAMERA_ZOOM * (ENEMY_HEIGHT / 128.0f));

		float hitX = (drawX + (ENEMY_WIDTH - ENEMY_HIT_WIDTH) / 2.0f - camera.pos.x) * CAMERA_ZOOM;
		float hitY = (drawY + (ENEMY_HEIGHT - ENEMY_HIT_HEIGHT) / 2.0f - camera.pos.y) * CAMERA_ZOOM;

		DrawBox((int)hitX, (int)hitY, (int)(hitX + ENEMY_HIT_WIDTH * CAMERA_ZOOM), (hitY + ENEMY_HIT_HEIGHT * CAMERA_ZOOM), GetColor(255, 0, 0),FALSE);
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

	StartAnimation(animData, enemy->pos.x, enemy->pos.y, 12, 9, 128, 128,true);

	animData->columNum = 3;
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