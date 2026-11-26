#include "EnemyB.h"
#include "../Map/MapChip.h"
#include "../Camera/Camera.h"

EnemyBdata g_EnemyB[ENEMY_B_MAX];

int g_EnemyBMapCount = 0;

struct EnemyBAnimationParam
{
    int interval;
    int frameNum;
    int frameWidth;
    int frameHeight;
};

const EnemyBAnimationParam ENEMY_B_ANIM_PARAM[ENEMY_B_ANIM_MAX] =
{
    { 12, 9, 128, 128 }, // IDLE
    { 12, 6, 128, 128 }, // MOVE
    { 12, 9, 128, 128 }, // ATTACK
};

void UpdateEnemyBAnimation(int enemyIndex);

void InitEnemyB()
{
    for (int i = 0; i < ENEMY_B_MAX; i++)
    {
        g_EnemyB[i].hp = ENEMY_B_MAX_HP;
        g_EnemyB[i].hitStopTimer = 0;
        g_EnemyB[i].isTurn = false;
        g_EnemyB[i].active = false;
        g_EnemyB[i].pos = VGet(0.0f, 0.0f, 0.0f);
        g_EnemyB[i].velocityX = 0.0f;
        g_EnemyB[i].velocityY = 0.0f;

        for (int j = 0; j < ENEMY_B_ANIM_MAX; j++)
        {
            InitAnimation(&g_EnemyB[i].animation[j]);
        }

        g_EnemyB[i].playAnim = ENEMY_B_ANIM_NONE;
    }
}

void LoadEnemyB()
{
    LoadEnemyBMap();

    g_EnemyB->animation[ENEMY_B_ANIM_IDLE].handle = LoadGraph("Data/Enemy/EnemyB/EnemyB_Idle.png");
    g_EnemyB->animation[ENEMY_B_ANIM_IDLE].invertedhandle = LoadGraph("Data/Enemy/EnemyB/EnemyB_Idle.png");
    GraphFilter(g_EnemyB->animation[ENEMY_B_ANIM_IDLE].invertedhandle, DX_GRAPH_FILTER_INVERT);

    g_EnemyB->animation[ENEMY_B_ANIM_MOVE].handle = LoadGraph("Data/Enemy/EnemyB/EnemyB_EnemyB_Move.png");
    g_EnemyB->animation[ENEMY_B_ANIM_MOVE].invertedhandle = LoadGraph("Data/Enemy/EnemyB/EnemyB_EnemyB_Move.png");
    GraphFilter(g_EnemyB->animation[ENEMY_B_ANIM_MOVE].invertedhandle, DX_GRAPH_FILTER_INVERT);

    g_EnemyB->animation[ENEMY_B_ANIM_ATTACK].handle = LoadGraph("Data/Enemy/EnemyB/EnemyB_Attack.png");
    g_EnemyB->animation[ENEMY_B_ANIM_ATTACK].invertedhandle = LoadGraph("Data/Enemy/EnemyB/EnemyB_Attack.png");
    GraphFilter(g_EnemyB->animation[ENEMY_B_ANIM_ATTACK].invertedhandle, DX_GRAPH_FILTER_INVERT);
}

void LoadEnemyBMap()
{
    FILE* fp;

    if (fopen_s(&fp, "Data/Map/LightMap.bin", "rb") != 0)return;

    g_EnemyBMapCount = 0;

    for (int i = 0; i < MAP_CHIP_Y_NUM; i++)
    {
        for (int j = 0; j < MAP_CHIP_X_NUM; j++)
        {
            int map = fgetc(fp);

            if (map == 3 && g_EnemyBMapCount < ENEMY_B_MAX)
            {
                g_EnemyB[g_EnemyBMapCount].hitStopTimer = 0;
                g_EnemyB[g_EnemyBMapCount].active = true;
                g_EnemyB[g_EnemyBMapCount].pos = VGet(j * MAP_CHIP_WIDTH, i * MAP_CHIP_HEIGHT, 0.0f);

                g_EnemyBMapCount++;
            }
        }
    }

    for (int i = 0; i < g_EnemyBMapCount; i++)
    {
        g_EnemyB[i].isTurn = false;
        g_EnemyB[i].playAnim = ENEMY_B_ANIM_NONE;

        for (int j = 0; j < ENEMY_B_ANIM_MAX; j++)
        {
            InitAnimation(&g_EnemyB[i].animation[j]);
        }
    }

    fclose(fp);
}

void StartEnemyB()
{
    for (int i = 0; i < g_EnemyBMapCount; i++)
    {
        StartEnemyBAnimation(i, ENEMY_B_ANIM_IDLE);
    }
}

void UpdateEnemyB()
{
}

void DrawEnemyB()
{
}

void FinEnemyB()
{
}

void StartEnemyBAnimation(int enemyIndex, EnemyBAnimationType anim)
{
    EnemyBdata* enemyB = &g_EnemyB[enemyIndex];

    if (anim == enemyB->playAnim)return;

    enemyB->playAnim = anim;

    AnimationData* animData = &enemyB->animation[anim];

    animData->nowFrame = 0;
    animData->timer = 0;

    const EnemyBAnimationParam* param = &ENEMY_B_ANIM_PARAM[anim];

    StartAnimation(animData, enemyB->pos.x, enemyB->pos.y,
        param->interval, param->frameNum,
        param->frameWidth, param->frameHeight, true);

    animData->columNum = 3;
}

void UpdateEnemyBAnimation(int enemyIndex)
{
    EnemyBdata* enemyB = &g_EnemyB[enemyIndex];

    EnemyBAnimationType animType = enemyB->playAnim;

    if (animType != ENEMY_B_ANIM_NONE)
    {
        AnimationData* animData = &enemyB->animation[animType];

        UpdateAnimation(animData);
    }
}
