
#include "DxLib.h"
#include "Player.h"
#include "../Input/Input.h"
#include "../Map/Block.h"
#include "../Animation/Animation.h"

PlayerData g_PlayerData = { 0 };

#define PLAYER_DEFAULT_POS_X 200.0f
#define PLAYER_DEFAULT_POS_Y 200.0f


static void SetPlayerAnimation(PlayerAnimationType type)
{
    if (g_PlayerData.playAnim == type)
    {
        return;
    }

    g_PlayerData.playAnim = type;

    AnimationData* anim = &g_PlayerData.animation[type];

    // アニメーションごとの画像サイズを維持する
    int frameNum = anim->frameNum;
    int width = anim->frameWidth;
    int height = anim->frameHeight;
    int interval = 1;

    if (type == PLAYER_ANIM_RUN)
    {
        interval = 6;
    }

    StartAnimation(
        anim,
        g_PlayerData.posX,
        g_PlayerData.posY,
        interval,
        frameNum,
        width,
        height,
        true
    );
}


void InitPlayer()
{
    g_PlayerData.active = false;
    g_PlayerData.isTurn = false;
    g_PlayerData.isAir = true;

    g_PlayerData.posX = PLAYER_DEFAULT_POS_X;
    g_PlayerData.posY = PLAYER_DEFAULT_POS_Y;

    g_PlayerData.moveX = 0.0f;
    g_PlayerData.moveY = 0.0f;
    g_PlayerData.speed = 4.0f;

    for (int i = 0; i < PLAYER_ANIM_MAX; i++)
    {
        InitAnimation(&g_PlayerData.animation[i]);
    }

    g_PlayerData.playAnim = PLAYER_ANIM_NONE;
}


void LoadPlayer()
{
    int idleImageWidth = 0;
    int idleImageHeight = 0;

    int runImageWidth = 0;
    int runImageHeight = 0;

    int idleImage = LoadGraph("Data/Player/DefaultPlayer.png");
    int runImage = LoadGraph("Data/Player/PlayerRun.png");

    if (idleImage == -1 || runImage == -1)
    {
        MessageBox(
            nullptr,
            "プレイヤー画像を読み込めませんでした。",
            "画像読み込みエラー",
            MB_OK
        );
        return;
    }

    GetGraphSize(idleImage, &idleImageWidth, &idleImageHeight);
    GetGraphSize(runImage, &runImageWidth, &runImageHeight);

    // 待機画像
    g_PlayerData.animation[PLAYER_ANIM_IDLE].handle = idleImage;
    g_PlayerData.animation[PLAYER_ANIM_IDLE].frameWidth = idleImageWidth;
    g_PlayerData.animation[PLAYER_ANIM_IDLE].frameHeight = idleImageHeight;
    g_PlayerData.animation[PLAYER_ANIM_IDLE].frameNum = 1;

    // 走行画像（横一列に9コマある場合）
    g_PlayerData.animation[PLAYER_ANIM_RUN].handle = runImage;
    g_PlayerData.animation[PLAYER_ANIM_RUN].frameWidth = runImageWidth / 9;
    g_PlayerData.animation[PLAYER_ANIM_RUN].frameHeight = runImageHeight;
    g_PlayerData.animation[PLAYER_ANIM_RUN].frameNum = 9;
}


void StartPlayer()
{
    g_PlayerData.active = true;
    g_PlayerData.playAnim = PLAYER_ANIM_NONE;

    SetPlayerAnimation(PLAYER_ANIM_IDLE);
}

void StepPlayer()
{
    // 左右移動
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

    BlockData* blocks = GetBlocks();

    // X方向の移動先
    float afterX = g_PlayerData.posX + g_PlayerData.moveX;

    // 左右の壁との衝突判定
    for (int i = 0; i < BLOCK_MAX; i++)
    {
        if (!blocks[i].active)
        {
            continue;
        }

        float blockLeft = blocks[i].pos.x;
        float blockTop = blocks[i].pos.y;
        float blockRight = blockLeft + MAP_CHIP_WIDTH;
        float blockBottom = blockTop + MAP_CHIP_HEIGHT;

        float playerLeft = afterX;
        float playerRight = afterX + PLAYER_WIDTH;
        float playerTop = g_PlayerData.posY;
        float playerBottom = playerTop + PLAYER_HEIGHT;

        if (playerBottom > blockTop &&
            playerTop < blockBottom)
        {
            // 右側の壁
            if (g_PlayerData.moveX > 0.0f &&
                g_PlayerData.posX + PLAYER_WIDTH <= blockLeft &&
                playerRight > blockLeft &&
                playerLeft < blockRight)
            {
                afterX = blockLeft - PLAYER_WIDTH;
            }

            // 左側の壁
            if (g_PlayerData.moveX < 0.0f &&
                g_PlayerData.posX >= blockRight &&
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

    g_PlayerData.posX = afterX;

    // ジャンプ
    if (IsInputKey(KEY_INPUT_SPACE) &&
        !g_PlayerData.isAir)
    {
        g_PlayerData.moveY = -12.0f;
        g_PlayerData.isAir = true;
    }

    // 重力
    g_PlayerData.moveY += 0.5f;

    float beforeY = g_PlayerData.posY;
    float afterY = beforeY + g_PlayerData.moveY;

    // 頭の衝突判定
    if (g_PlayerData.moveY < 0.0f)
    {
        for (int i = 0; i < BLOCK_MAX; i++)
        {
            if (!blocks[i].active)
            {
                continue;
            }

            float blockLeft = blocks[i].pos.x;
            float blockTop = blocks[i].pos.y;
            float blockRight = blockLeft + MAP_CHIP_WIDTH;
            float blockBottom = blockTop + MAP_CHIP_HEIGHT;

            float playerLeft = g_PlayerData.posX;
            float playerRight = playerLeft + PLAYER_WIDTH;

            if (playerRight > blockLeft &&
                playerLeft < blockRight &&
                beforeY >= blockBottom &&
                afterY <= blockBottom)
            {
                afterY = blockBottom;
                g_PlayerData.moveY = 0.0f;
                break;
            }
        }
    }

    // 着地判定
    bool onGround = false;

    for (int i = 0; i < BLOCK_MAX; i++)
    {
        if (!blocks[i].active)
        {
            continue;
        }

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

    // アニメーション選択
    if (g_PlayerData.isAir)
    {
        if (g_PlayerData.moveY < 0.0f)
        {
            SetPlayerAnimation(PLAYER_ANIM_JUMP);
        }
        else
        {
            SetPlayerAnimation(PLAYER_ANIM_FALL);
        }
    }
    else if (g_PlayerData.moveX != 0.0f)
    {
        SetPlayerAnimation(PLAYER_ANIM_RUN);
    }
    else
    {
        SetPlayerAnimation(PLAYER_ANIM_IDLE);
    }

    // 現在のアニメーションを更新
    AnimationData* anim =
        &g_PlayerData.animation[g_PlayerData.playAnim];

    // 描画座標をプレイヤーに合わせる
    anim->posX = g_PlayerData.posX;
    anim->posY = g_PlayerData.posY;

    UpdateAnimation(anim);
}

void DrawPlayer()
{
    if (!g_PlayerData.active)
    {
        return;
    }

    AnimationData* anim =
        &g_PlayerData.animation[g_PlayerData.playAnim];

    if (!anim->active || anim->handle == -1)
    {
        return;
    }

    int frameX = anim->frameWidth * anim->nowFrame;

    // 40×60のプレイヤーサイズに合わせて描画

    DrawRectExtendGraph(
        (int)g_PlayerData.posX,
        (int)g_PlayerData.posY,
        (int)(g_PlayerData.posX + PLAYER_WIDTH),
        (int)(g_PlayerData.posY + PLAYER_HEIGHT),
        frameX,
        0,
        frameX + anim->frameWidth,
        anim->frameHeight,
        anim->handle,
        TRUE
    );

}

void FinPlayer()
{
    for (int i = 0; i < PLAYER_ANIM_MAX; i++)
    {
        int handle = g_PlayerData.animation[i].handle;

        if (handle != -1 && handle != 0)
        {
            DeleteGraph(handle);
            g_PlayerData.animation[i].handle = -1;
        }
    }
}

PlayerData GetPlayer()
{
    return g_PlayerData;
}
