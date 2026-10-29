#include"DxLib.h" 
#include "Map/MapManager.h"
#include "../Source/Map/Block.h"
#include "Input/Input.h"
#include "Player/Player.h"

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

    LoadPlayer();
    LoadMap();

    StartMap();
    StartPlayer();

    SetDrawScreen(DX_SCREEN_BACK);

    while (ProcessMessage() >= 0)
    {
        ClearDrawScreen();

        UpdateInput();
        UpdatePlayer();

        DrawBox(0, 0, 1280, 720,
            GetColor(255, 255, 255), TRUE);

        if (CheckHitKey(KEY_INPUT_ESCAPE))
        {
            break;
        }

        DrawMap();
        DrawPlayer();

        ScreenFlip();
    }
}