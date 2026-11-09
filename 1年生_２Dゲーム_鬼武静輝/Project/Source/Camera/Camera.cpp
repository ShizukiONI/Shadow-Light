#include "DxLib.h"
#include "Camera.h"
#include "../Player/Player.h"
#include "../Map/MapChip.h"

// カメラデータ
CameraData g_CameraData = { 0 };

// 初期化
void InitCamera()
{
    g_CameraData.pos = VGet(0.0f, 0.0f, 0.0f);
}

// カメラ更新
void StepCamera()
{
    PlayerData player = GetPlayer();

    int screenWidth;
    int screenHeight;
    int colorBitDepth;

    GetScreenState(&screenWidth, &screenHeight, &colorBitDepth);

    // プレイヤーが画面中央に来るようにする
    g_CameraData.pos.x =
        player.pos.x + PLAYER_WIDTH / 2.0f - screenWidth / 2.0f;

    g_CameraData.pos.y =
        player.pos.y + PLAYER_HEIGHT / 2.0f - screenHeight / 2.0f;

    // カメラがマップの左端を超えないようにする
    if (g_CameraData.pos.x < 0.0f)
    {
        g_CameraData.pos.x = 0.0f;
    }

    // カメラがマップの右端を超えないようにする
    float mapWidth = MAP_CHIP_X_NUM * MAP_CHIP_WIDTH;

    if (g_CameraData.pos.x > mapWidth - screenWidth)
    {
        g_CameraData.pos.x = mapWidth - screenWidth;
    }

    // カメラがマップの下端を超えないようにする
    float mapHeight = MAP_CHIP_Y_NUM * MAP_CHIP_HEIGHT;

    if (g_CameraData.pos.y > mapHeight - screenHeight)
    {
        g_CameraData.pos.y = mapHeight - screenHeight;
    }
}

// カメラ情報取得
CameraData GetCamera()
{
    return g_CameraData;
}