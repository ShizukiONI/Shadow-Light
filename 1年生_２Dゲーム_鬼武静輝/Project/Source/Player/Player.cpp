
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
    //左右移動
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

   /* g_PlayerData.posX += g_PlayerData.moveX;*/

    //移動後のX座標を計算
    float afterX = g_PlayerData.posX + g_PlayerData.moveX;

    //ブロックとの左右の当たり判定
    BlockData* blocks = GetBlocks();

    for (int i = 0; i < BLOCK_MAX; i++)
    {
        if (!blocks[i].active)
            continue;

        float blockLeft = blocks[i].pos.x;
        float blockTop = blocks[i].pos.y;
        float blockRight = blockLeft + MAP_CHIP_WIDTH;
        float blockBottom = blockTop + MAP_CHIP_HEIGHT;

        float playerLeft = afterX;
        float playerRight = afterX + PLAYER_WIDTH;
        float playerTop = g_PlayerData.posY;
        float playerBottom = playerTop + PLAYER_HEIGHT;

        //プレイヤーとブロックが上下方向に重なっているか
        if (playerBottom > blockTop && playerTop < blockBottom)
        {
            //右側の壁にぶつかった場合
            if (g_PlayerData.moveX > 0.0f &&
                g_PlayerData.posX + PLAYER_WIDTH <= blockLeft &&
                playerRight > blockLeft &&
                playerLeft < blockRight)
            {
                afterX = blockLeft - PLAYER_WIDTH;
            }

            //左側の壁にぶつかった場合
            if (g_PlayerData.moveX < 0.0f &&
                g_PlayerData.posX + PLAYER_WIDTH >= blockRight &&
                playerLeft < blockRight &&
                playerRight > blockLeft)
            {
                afterX = blockRight;
            }
        }
    }

    // 画面端の制限
    if (afterX < 0.0f)
    {
        afterX = 0.0f;
    }

    if (afterX > 1280.0f - PLAYER_WIDTH)
    {
        afterX = 1280.0f - PLAYER_WIDTH;
    }

    // 計算した座標を反映
    g_PlayerData.posX = afterX;

    //スペースキーでジャンプ
    if (IsInputKey(KEY_INPUT_SPACE) &&
        !g_PlayerData.isAir)
    {
        g_PlayerData.moveY = -12.0f;
        g_PlayerData.isAir = true;
    }

    //重力
    g_PlayerData.moveY += 0.5f;

    //落下後のY座標
    float beforeY = g_PlayerData.posY;
    float afterY = beforeY + g_PlayerData.moveY;

    //ブロックと頭の衝突判定
    if (g_PlayerData.moveY < 0.0f)
    {
        for (int i = 0; i < BLOCK_MAX; i++)
        {
            if (!blocks[i].active)
                continue;

            float blockLeft = blocks[i].pos.x;
            float blockTop = blocks[i].pos.y;
            float blockRight = blockLeft + MAP_CHIP_WIDTH;

            float playerLeft = g_PlayerData.posX;
            float playerRight = playerLeft + PLAYER_WIDTH;

            float beforeTop = beforeY;
            float afterTop = afterY;

            // 頭がブロックの下面にぶつかった場合
            if (playerRight > blockLeft &&
                playerLeft < blockRight &&
                beforeTop >= blockTop + MAP_CHIP_HEIGHT &&
                afterTop <= blockTop + MAP_CHIP_HEIGHT)
            {
                afterY = blockTop + MAP_CHIP_HEIGHT;
                g_PlayerData.moveY = 0.0f;
                break;
            }
        }
    }

    bool onGround = false;

    //ブロックとの着地判定
    for (int i = 0; i < BLOCK_MAX; i++)
    {
        if (!blocks[i].active)
            continue;

        float blockLeft = blocks[i].pos.x;
        float blockTop = blocks[i].pos.y;
        float blockRight = blockLeft + MAP_CHIP_WIDTH;

        float playerLeft = g_PlayerData.posX;
        float playerRight = playerLeft + PLAYER_WIDTH;

        float beforeBottom = beforeY + PLAYER_HEIGHT;
        float afterBottom = afterY + PLAYER_HEIGHT;

        if (g_PlayerData.moveY >= 0.0f &&
            playerRight > blockLeft &&
            playerLeft < blockRight &&
            beforeBottom <= blockTop &&
            afterBottom >= blockTop)
        {
            afterY = blockTop - PLAYER_HEIGHT;
            g_PlayerData.moveY = 0.0f;
            onGround = true;
            break;
        }

        ////すでにブロックの上に立っている場合
        //if (g_PlayerData.moveY >= 0.0f &&
        //    beforeBottom >= blockTop - 1.0f &&
        //    beforeBottom <= blockTop + 1.0f &&
        //    afterBottom >= blockTop)
        //{
        //    afterY = blockTop - PLAYER_HEIGHT;
        //    g_PlayerData.moveY = 0.0f;
        //    onGround = true;
        //    break;
        //}
    }

    g_PlayerData.posY = afterY;
    g_PlayerData.isAir = !onGround;

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

    DrawExtendGraph(
        (int)g_PlayerData.posX,
        (int)g_PlayerData.posY,
        (int)(g_PlayerData.posX + PLAYER_WIDTH),
        (int)(g_PlayerData.posY + PLAYER_HEIGHT),
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
