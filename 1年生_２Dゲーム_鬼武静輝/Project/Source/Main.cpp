#include"DxLib.h" 
#include "Map/MapManager.h"
#include "../Source/Map/Block.h"
#include "Input/Input.h"
#include "../Source/Enemy/EnemyA.h"
#include "Player/Player.h"
#include "../Source/World/World.h"
#include "../Source/Camera/Camera.h"

void DrawWorldScene(WorldType world)
{
    // 現在の世界を保存
    WorldType saveWorld = g_WorldType;

    // 描画する世界を変更
    g_WorldType = world;

    // 背景・ブロック・プレイヤーを描画
    DrawWorldBackGround(world);
    DrawMap();
    DrawEnemy();
    DrawPlayer();

    // 元の世界に戻す
    g_WorldType = saveWorld;
}

void SaveWorldScreen(int screenHandle, WorldType world)
{
    // 保存先のスクリーンに切り替える
    SetDrawScreen(screenHandle);

    // 画面をクリア
    ClearDrawScreen();

    // 指定した世界を描画
    DrawWorldScene(world);
}

void DrawWorldTransition()
{
    if (g_WorldEffectTimer > 0)
    {
        // 切り替え前と切り替え後の世界を保存
        SaveWorldScreen(g_WorldBeforeScreen, g_WorldBeforeEffect);
        SaveWorldScreen(g_WorldAfterScreen, g_WorldAfterEffect);

        // 描画先を画面に戻す
        SetDrawScreen(DX_SCREEN_BACK);

        // まず切り替え前の世界を全面に描画
        DrawGraph(0, 0, g_WorldBeforeScreen, FALSE);

        // 新しい世界を表示する範囲
        int revealX = (int)(g_WorldEffectX + 1280.0f);

        if (revealX < 0) revealX = 0;
        if (revealX > 1280) revealX = 1280;

        // 左側から新しい世界を表示
        SetDrawArea(0, 0, revealX, 720);
        DrawGraph(0, 0, g_WorldAfterScreen, FALSE);

        // 描画範囲を元に戻す
        SetDrawArea(0, 0, 1280, 720);
    }
    else
    {
        SetDrawScreen(DX_SCREEN_BACK);
        DrawWorldScene(g_WorldType);
    }
}

//プログラムの初めはWinMainから始まります
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstamce, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    SetGraphMode(1280, 720, 32);
    ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)  //DXライブラリ初期化処理
	{
		return -1;           //エラーが起きたら直ちに終了
	}

    InitBlock();
    InitMap();
    InitInput();
    InitPlayer();
    InitEnemy();
    InitWorld();
    InitCamera();

    LoadPlayer();
    LoadMap();
    LoadEnemy();

    StartMap();
    StartPlayer();
    StartEnemy();

    SetDrawScreen(DX_SCREEN_BACK);

    while (ProcessMessage() >= 0)
    {
        ClearDrawScreen();

        UpdateInput();
        UpdatePlayer();
        UpdateEnemy();
        UpdateWorld();
        UpdateWorldEffect();
        StepCamera();

        if (CheckHitKey(KEY_INPUT_ESCAPE))
        {
            break;
        }

        DrawWorldTransition();

        ScreenFlip();
    }
}