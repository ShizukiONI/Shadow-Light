
#include "DxLib.h"
#include "Player.h"
#include "../Input/Input.h"
#include "../Map/Block.h"

PlayerData g_PlayerData = { 0 };

int g_playerImage = -1;

#define PLAYER_DEFAULT_POS_X ((980.0f - PLAYER_WIDTH) / 2.0f)
#define PLAYER_DEFAULT_POS_Y 500.0f
void InitPlayer()
{
    g_PlayerData.active = true;
    g_PlayerData.isTurn = false;
    g_PlayerData.isAir = true;

    g_PlayerData.posX = 200.0f;
    g_PlayerData.posY = 200.0f;

    g_PlayerData.moveX = 0.0f;
    g_PlayerData.moveY = 0.0f;
    g_PlayerData.speed = 4.0f;
}

void LoadPlayer()
{
    g_playerImage = LoadGraph("Data/Player/DefaultPlayer.png");
}

void StartPlayer()
{
    g_PlayerData.active = true;
}

void StepPlayer()
{
    g_PlayerData.moveX = 0.0f;

    if (IsInputKey(KEY_INPUT_LEFT) ||
        IsInputKey(KEY_INPUT_A))
    {
        g_PlayerData.moveX = -g_PlayerData.speed;
        g_PlayerData.isTurn = true;
    }

    if (IsInputKey(KEY_INPUT_RIGHT) ||
        IsInputKey(KEY_INPUT_D))
    {
        g_PlayerData.moveX = g_PlayerData.speed;
        g_PlayerData.isTurn = false;
    }

    g_PlayerData.posX += g_PlayerData.moveX;

    if (g_PlayerData.posX < 0.0f)
    {
        g_PlayerData.posX = 0.0f;
    }

    if (g_PlayerData.posX > 1280.0f - PLAYER_WIDTH)
    {
        g_PlayerData.posX = 1280.0f - PLAYER_WIDTH;
    }
}

void UpdatePlayer()
{
    if (!g_PlayerData.active)
    {
        return;
    }

    StepPlayer();
}


void DrawPlayer()
{
    if (!g_PlayerData.active)
    {
        return;
    }
    if (g_playerImage == -1)
    {
        MessageBox(
            nullptr,
            "プレイヤー画像を読み込めませんでした。\n"
            "実行ファイルの場所と画像ファイルの配置を確認してください。",
            "画像読み込みエラー",
            MB_OK
        );
    }

    if (!g_PlayerData.active || g_playerImage == -1)return;

    DrawGraph(
        (int)g_PlayerData.posX,
        (int)g_PlayerData.posY,
        g_playerImage,
        TRUE
    );
}


void FinPlayer()
{
}

PlayerData GetPlayer()
{
    return g_PlayerData;
}
