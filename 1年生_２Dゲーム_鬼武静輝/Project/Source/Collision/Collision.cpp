#include "DxLib.h"
#include "Collision.h"
#include "../Player/Player.h"
#include "../Enemy/EnemyA.h"
#include "../Map/MapParameter.h"
#include "../Map/MapChip.h"
#include "../World/World.h"

bool CheckPlayerBlockCollision(VECTOR playerPos, int mapX, int mapY)
{
    // マップ範囲外なら衝突しない
    if (mapX < 0 || mapX >= MAP_CHIP_X_NUM ||
        mapY < 0 || mapY >= MAP_CHIP_Y_NUM)
    {
        return false;
    }

    MapChipData chip = GetMapChipData(mapX, mapY, g_WorldType);

    //現在の世界と違うブロックなら衝突しない
    if (chip.data == nullptr ||
        chip.data->world != g_WorldType)
    {
        return false;
    }

    float blockLeft = mapX * MAP_CHIP_WIDTH;
    float blockTop = mapY * MAP_CHIP_HEIGHT;

    float blockRight = blockLeft + MAP_CHIP_WIDTH;
    float blockBottom = blockTop + MAP_CHIP_HEIGHT;

    float playerLeft = playerPos.x +
        (PLAYER_WIDTH - PLAYER_HIT_WIDTH) / 2.0f;

    float playerTop = playerPos.y + 8.0f +
        (PLAYER_HEIGHT - PLAYER_HIT_HEIGHT) / 2.0f;

    float playerRight = playerLeft + PLAYER_HIT_WIDTH;
    float playerBottom = playerTop + PLAYER_HIT_HEIGHT;

    return (
        playerRight > blockLeft &&
        playerLeft < blockRight &&
        playerBottom > blockTop &&
        playerTop < blockBottom
        );
}

float CheckPlayerCollisionX(float nextX, float playerY)
{

    PlayerData player = GetPlayer();

    float hitX = (PLAYER_WIDTH - PLAYER_HIT_WIDTH) / 2.0f;
    float hitY = 8.0f + (PLAYER_HEIGHT - PLAYER_HIT_HEIGHT) / 2.0f;

    int startX = (int)((nextX + hitX) / MAP_CHIP_WIDTH);
    int endX = (int)((nextX + hitX + PLAYER_HIT_WIDTH - 1) / MAP_CHIP_WIDTH);

    int startY = (int)((playerY + hitY) / MAP_CHIP_HEIGHT);
    int endY = (int)((playerY + hitY + PLAYER_HIT_HEIGHT - 1) / MAP_CHIP_HEIGHT);

    for (int y = startY; y <= endY; y++)
    {
        for (int x = startX; x <= endX; x++)
        {
            if (!CheckPlayerBlockCollision(
                VGet(nextX, playerY, 0.0f), x, y))
            {
                continue;
            }

            float hitX = (PLAYER_WIDTH - PLAYER_HIT_WIDTH) / 2.0f;

            if (nextX > player.pos.x)
            {
                // 右方向の衝突
                return x * MAP_CHIP_WIDTH - PLAYER_HIT_WIDTH - hitX;
            }
            else if (nextX < player.pos.x)
            {
                // 左方向の衝突
                return (x + 1) * MAP_CHIP_WIDTH - hitX;
            }
        }
    }

    return nextX;
}

float CheckPlayerCollisionY(float nextY, float playerX)
{

    PlayerData player = GetPlayer();

    float hitX = (PLAYER_WIDTH - PLAYER_HIT_WIDTH) / 2.0f;
    float hitY = 8.0f + (PLAYER_HEIGHT - PLAYER_HIT_HEIGHT) / 2.0f;

    int startX = (int)((playerX + hitX) / MAP_CHIP_WIDTH);
    int endX = (int)((playerX + hitX + PLAYER_HIT_WIDTH - 1) / MAP_CHIP_WIDTH);

    int startY = (int)((nextY + hitY) / MAP_CHIP_HEIGHT);
    int endY = (int)((nextY + hitY + PLAYER_HIT_HEIGHT - 0.001f) / MAP_CHIP_HEIGHT);

    for (int y = startY; y <= endY; y++)
    {
        for (int x = startX; x <= endX; x++)
        {
            if (!CheckPlayerBlockCollision(
                VGet(playerX, nextY, 0.0f), x, y))
            {
                continue;
            }

            if (nextY > player.pos.y)
            {
                // 下方向の衝突
                return y * MAP_CHIP_HEIGHT - PLAYER_HIT_HEIGHT - hitY;
            }
            else if (nextY < player.pos.y)
            {
                // 上方向の衝突
                return (y + 1) * MAP_CHIP_HEIGHT - hitY;
            }
        }
    }

    return nextY;
}

int CheckPlayerEnemyACollision()
{
    PlayerData player = GetPlayer();

    if (!player.active)
    {
        return false;
    }

    float playerLeft = player.pos.x + (PLAYER_WIDTH - PLAYER_HIT_WIDTH) / 2.0f;
    float playerTop = player.pos.y + 8.0f + (PLAYER_HEIGHT - PLAYER_HIT_HEIGHT) / 2.0f;

    float playerRight = playerLeft + PLAYER_HIT_WIDTH;
    float playerBottom = playerTop + PLAYER_HIT_HEIGHT;

    for (int i = 0; i < g_EnemyMapCount; i++)
    {
        EnemyData* enemy = &g_Enemy[i];

        if (!enemy->active)
        {
            continue;
        }

        float enemyLeft = enemy->pos.x + (MAP_CHIP_WIDTH - ENEMY_WIDTH) / 2.0f + (ENEMY_WIDTH - ENEMY_HIT_WIDTH) / 2.0f;
        float enemyTop = enemy->pos.y + (MAP_CHIP_HEIGHT - ENEMY_HEIGHT) / 2.0f + (ENEMY_HEIGHT - ENEMY_HIT_HEIGHT) / 2.0f;

        float enemyRight = enemyLeft + ENEMY_HIT_WIDTH;
        float enemyBottom = enemyTop + ENEMY_HIT_HEIGHT;

        if (playerRight > enemyLeft && playerLeft < enemyRight && playerBottom > enemyTop && playerTop < enemyBottom)
        {
            return i;
        }
    }

    return -1;
}

bool CheckPlayerAttackEnemyCollision(int enemyIndex)
{
    PlayerData player = GetPlayer();

    if (!player.active || enemyIndex < 0 || enemyIndex >= g_EnemyMapCount)
    {
        return false;
    }

    EnemyData* enemy = &g_Enemy[enemyIndex];

    if (!enemy->active)
    {
        return false;
    }

    float playerLeft = player.pos.x + (PLAYER_WIDTH - PLAYER_HIT_WIDTH) / 2.0f;
    float playerTop = player.pos.y + 8.0f + (PLAYER_HEIGHT - PLAYER_HIT_HEIGHT) / 2.0f;


    return false;
}

bool CheckEnemyABlockCollision(VECTOR enemyApos, int mapX, int mapY)
{
    // マップ範囲外なら衝突しない
    if (mapX < 0 || mapX >= MAP_CHIP_X_NUM ||
        mapY < 0 || mapY >= MAP_CHIP_Y_NUM)
    {
        return false;
    }

    MapChipData chip = GetMapChipData(mapX, mapY, g_WorldType);

    //現在の世界と違うブロックなら衝突しない
    if (chip.data == nullptr ||
        chip.data->world != g_WorldType)
    {
        return false;
    }

    float blockLeft = mapX * MAP_CHIP_WIDTH;
    float blockTop = mapY * MAP_CHIP_HEIGHT;

    float blockRight = blockLeft + MAP_CHIP_WIDTH;
    float blockBottom = blockTop + MAP_CHIP_HEIGHT;

    float enemyLeft = enemyApos.x + (ENEMY_WIDTH - ENEMY_HIT_WIDTH) / 2.0f;

    float enemyTop = enemyApos.y + (ENEMY_HEIGHT - ENEMY_HIT_HEIGHT) / 2.0f;

    float enemyRight = enemyLeft + ENEMY_HIT_WIDTH;
    float enemyBottom = enemyTop + ENEMY_HIT_HEIGHT;

    return(enemyRight > blockLeft && enemyLeft < blockRight && enemyBottom > blockTop && enemyTop < blockBottom);

    return false;
}

float CheckEnemyACollisiomX(float nextX, float enemyAY, float currentX)
{
    float hitX = (ENEMY_WIDTH - ENEMY_HIT_WIDTH) / 2.0f;
    float hitY = (ENEMY_HEIGHT - ENEMY_HIT_HEIGHT) / 2.0f;

    int startX = (int)((nextX + hitX) / MAP_CHIP_WIDTH);
    int endX = (int)((nextX + hitX + ENEMY_HIT_WIDTH - 1) / MAP_CHIP_WIDTH);

    int startY = (int)((enemyAY + hitY) / MAP_CHIP_HEIGHT);
    int endY = (int)((enemyAY + hitY + ENEMY_HIT_HEIGHT - 1) / MAP_CHIP_HEIGHT);

    for (int y = startY; y <= endY; y++)
    {
        for (int x = startX; x <= endX; x++)
        {
            if (!CheckEnemyABlockCollision(VGet(nextX, enemyAY, 0.0f), x, y))
            {
                continue;
            }

            if (nextX > currentX)
            {
                return x * MAP_CHIP_WIDTH - ENEMY_HIT_WIDTH - hitX;
            }
            else if (nextX < currentX)
            {
                return(x + 1) * MAP_CHIP_WIDTH - hitX;
            }
        }
    }

    return nextX;
}

float CheckEnemyACollisionY(float nextY, float enemyX, float currentY)
{
    float hitX = (ENEMY_WIDTH - ENEMY_HIT_WIDTH) / 2.0f;
    float hitY = (ENEMY_HEIGHT - ENEMY_HIT_HEIGHT) / 2.0f;

    int startX = (int)((enemyX + hitX) / MAP_CHIP_WIDTH);
    int endX = (int)((enemyX + hitX + ENEMY_HIT_WIDTH - 1) / MAP_CHIP_WIDTH);

    int startY = (int)((nextY + hitY) / MAP_CHIP_HEIGHT);
    int endY = (int)((nextY + hitY + ENEMY_HIT_HEIGHT - 0.001f) / MAP_CHIP_HEIGHT);

    for (int y = startY; y <= endY; y++)
    {
        for (int x = startX; x <= endX; x++)
        {
            if (!CheckEnemyABlockCollision(
                VGet(enemyX, nextY, 0.0f), x, y))
            {
                continue;
            }

            if (nextY > currentY)
            {
                // 下方向の衝突
                return y * MAP_CHIP_HEIGHT - ENEMY_HIT_HEIGHT - hitY;
            }
            else if (nextY < currentY)
            {
                // 上方向の衝突
                return (y + 1) * MAP_CHIP_HEIGHT - hitY;
            }
        }
    }

    return nextY;
}