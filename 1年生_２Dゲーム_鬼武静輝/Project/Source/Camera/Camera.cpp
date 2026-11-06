#include "DxLib.h"
#include "Camera.h"
#include "../Player/Player.h"

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
}

// カメラ情報取得
CameraData GetCamera()
{
    return g_CameraData;
}