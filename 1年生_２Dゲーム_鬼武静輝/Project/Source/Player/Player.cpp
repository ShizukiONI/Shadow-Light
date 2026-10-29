#include "DxLib.h"
#include "Player.h"
#include "../Input/Input.h"
#include "../Map/Block.h"
#include "../Animation/Animation.h"
#include "../Collision/Collision.h"


struct PlayerAnimationParam
{
    int interval;
    int frameNum;
    int width;
    int height;
};

const PlayerAnimationParam PLAYER_ANIM_PARAM[PLAYER_ANIM_MAX] =
{
    { 20, 4, 65, 60 },    // 待機：画像読み込み時に設定
    { 9, 8, 64, 60 },    // 走行：画像読み込み時に設定
    { 8, 4, 60, 60 },    // ジャンプ
    { 8, 1, 50, 60 },    // 落下
    { 1, 1, 60, 60 },    // 着地
};


PlayerData g_PlayerData = { 0 };

// このCPPでのみ使用する関数の宣言
void StartPlayerAnimation(PlayerAnimationType anim);	// アニメーション再生
void UpdatePlayerAnimation();							// アニメーション更新

#define PLAYER_DEFAULT_POS_X 200.0f
#define PLAYER_DEFAULT_POS_Y 200.0f

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
    int idleImageWidth = 36;
    int idleImageHeight = 60;

    int runImageWidth = 32;
    int runImageHeight = 60;

    g_PlayerData.animation[PLAYER_ANIM_RUN].handle = LoadGraph("Data/Player/PlayerRunNeko3.png");
    g_PlayerData.animation[PLAYER_ANIM_IDLE].handle = LoadGraph("Data/Player/PlayerIdle2.png");
    g_PlayerData.animation[PLAYER_ANIM_JUMP].handle = LoadGraph("Data/Player/PlayerJump2.png");
    g_PlayerData.animation[PLAYER_ANIM_FALL].handle = LoadGraph("Data/Player/PlayerFall2.png");
}


void StartPlayer()
{
    g_PlayerData.active = true;

    // 待機アニメーションから開始する
    StartPlayerAnimation(PLAYER_ANIM_IDLE);

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

        // 移動先のPlayerの当たり判定
        float playerLeft = afterX;
        float playerRight = afterX + PLAYER_WIDTH;
        float playerTop = g_PlayerData.posY;
        float playerBottom = playerTop + PLAYER_HEIGHT;

        // Collisionの共通関数で衝突を確認
        bool isCollision = CheckCollisionBox(
            playerLeft, playerTop,
            playerRight, playerBottom,
            blockLeft, blockTop,
            blockRight, blockBottom
        );

        if (!isCollision)
        {
            continue;
        }

        // 右へ移動して壁に衝突した場合
        if (g_PlayerData.moveX > 0.0f)
        {
            afterX = blockLeft - PLAYER_WIDTH;
        }
        // 左へ移動して壁に衝突した場合
        else if (g_PlayerData.moveX < 0.0f)
        {
            afterX = blockRight;
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

    UpdatePlayerAnimation();
}


void DrawPlayer()
{
    if (!g_PlayerData.active)
    {
        return;
    }

    // アニメーション描画でプレイヤーを描画する
    PlayerAnimationType animType = g_PlayerData.playAnim;
    AnimationData* animData = &g_PlayerData.animation[animType];
    DrawAnimation(animData, g_PlayerData.posX, g_PlayerData.posY , g_PlayerData.isTurn);

}

void FinPlayer()
{
    for (int i = 0; i < PLAYER_ANIM_MAX; i++)
    {
        DeleteGraph(g_PlayerData.animation[i].handle);
    }
}

PlayerData GetPlayer()
{
    return g_PlayerData;
}


void StartPlayerAnimation(PlayerAnimationType anim)
{
    // 再生中のアニメーションであれば何もしない
    if (anim == g_PlayerData.playAnim) return;

    // 再生中アニメーション設定
    g_PlayerData.playAnim = anim;

    // 再生に必要なデータを取得
    AnimationData* animData = &g_PlayerData.animation[anim];
    PlayerAnimationParam animParam = PLAYER_ANIM_PARAM[anim];

    // アニメーション再生
    StartAnimation(animData, g_PlayerData.posX, g_PlayerData.posY, animParam.interval, animParam.frameNum, animParam.width, animParam.height, true);
}

void UpdatePlayerAnimation()
{
    // 地上にいるか
    if (!g_PlayerData.isAir)
    {
        // 横に移動していれば走っている
        if (g_PlayerData.moveX < 0.0f || g_PlayerData.moveX > 0.0f)
        {
            // 走るアニメーション
            StartPlayerAnimation(PLAYER_ANIM_RUN);
        }
        else
        {
            // 移動していなければ待機
            StartPlayerAnimation(PLAYER_ANIM_IDLE);
        }
    }
    // 空中にいる
    else
    {
        // 上昇していればジャンプアニメーション
        if (g_PlayerData.moveY < 0.0f)
        {
            StartPlayerAnimation(PLAYER_ANIM_JUMP);
        }
        // 下降していれば落下アニメーション
        else if (g_PlayerData.moveY > 0.0f)
        {
            StartPlayerAnimation(PLAYER_ANIM_FALL);
        }
    }

    // アニメーション更新
    PlayerAnimationType animType = g_PlayerData.playAnim;
    AnimationData* animData = &g_PlayerData.animation[animType];
    UpdateAnimation(animData);
}