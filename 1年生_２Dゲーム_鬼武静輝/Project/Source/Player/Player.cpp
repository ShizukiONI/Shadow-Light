
#include "DxLib.h"
#include "Player.h"
#include "../Input/Input.h"
#include "../Map/MapChip.h"
#include "../Animation/Animation.h"
#include "../World/World.h"
#include "../Camera/Camera.h"


struct PlayerAnimationParam
{
    int interval;
    int frameNum;
    int width;
    int height;
};

const PlayerAnimationParam PLAYER_ANIM_PARAM[PLAYER_ANIM_MAX] =
{
    { 15, 4, 65, 60 }, // IDLE
    { 9,  8, 64, 60 }, // RUN
    { 8,  4, 67, 60 }, // JUMP
    { 8,  1, 60, 60 }, // FALL
    { 10, 1, 92, 60 }, // LANDING
    { 10, 1, 80, 60 }, //ATTACK
};


PlayerData g_PlayerData = { 0 };

// このCPPでのみ使用する関数の宣言
void StartPlayerAnimation(PlayerAnimationType anim);	// アニメーション再生
void UpdatePlayerAnimation();							// アニメーション更新

#define PLAYER_DEFAULT_POS_X 100.0f
#define PLAYER_DEFAULT_POS_Y 900.0f

void InitPlayer()
{
    g_PlayerData.active = false;
    g_PlayerData.isTurn = false;
    g_PlayerData.isAir = true;

    g_PlayerData.pos = VGet(PLAYER_DEFAULT_POS_X, PLAYER_DEFAULT_POS_Y, 0.0f);

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
    g_PlayerData.animation[PLAYER_ANIM_RUN].handle = LoadGraph("Data/Player/PlayerRunNeko3.png");
    g_PlayerData.animation[PLAYER_ANIM_RUN].invertedhandle = LoadGraph("Data/Player/PlayerRunNeko3.png");
    GraphFilter(g_PlayerData.animation[PLAYER_ANIM_RUN].invertedhandle, DX_GRAPH_FILTER_INVERT);
    

    g_PlayerData.animation[PLAYER_ANIM_IDLE].handle = LoadGraph("Data/Player/PlayerIdle2.png");
    g_PlayerData.animation[PLAYER_ANIM_IDLE].invertedhandle = LoadGraph("Data/Player/PlayerIdle2.png");
    GraphFilter(g_PlayerData.animation[PLAYER_ANIM_IDLE].invertedhandle, DX_GRAPH_FILTER_INVERT);

    g_PlayerData.animation[PLAYER_ANIM_JUMP].handle = LoadGraph("Data/Player/PlayerJump2.png");
    g_PlayerData.animation[PLAYER_ANIM_JUMP].invertedhandle = LoadGraph("Data/Player/PlayerJump2.png");
    GraphFilter(g_PlayerData.animation[PLAYER_ANIM_JUMP].invertedhandle, DX_GRAPH_FILTER_INVERT);


    g_PlayerData.animation[PLAYER_ANIM_FALL].handle = LoadGraph("Data/Player/PlayerFall2.png");
    g_PlayerData.animation[PLAYER_ANIM_FALL].invertedhandle = LoadGraph("Data/Player/PlayerFall2.png");
    GraphFilter(g_PlayerData.animation[PLAYER_ANIM_FALL].invertedhandle, DX_GRAPH_FILTER_INVERT);


    g_PlayerData.animation[PLAYER_ANIM_LANDING].handle = LoadGraph("Data/Player/Tyakuti2.png");
    g_PlayerData.animation[PLAYER_ANIM_LANDING].invertedhandle = LoadGraph("Data/Player/Tyakuti2.png");
    GraphFilter(g_PlayerData.animation[PLAYER_ANIM_LANDING].invertedhandle, DX_GRAPH_FILTER_INVERT);

    g_PlayerData.animation[PLAYER_ANIM_ATTACK].handle = LoadGraph("Data/Player/Attack2.png");
    g_PlayerData.animation[PLAYER_ANIM_ATTACK].invertedhandle = LoadGraph("Data/Player/Attack2.png");
    GraphFilter(g_PlayerData.animation[PLAYER_ANIM_ATTACK].invertedhandle, DX_GRAPH_FILTER_INVERT);

}


void StartPlayer()
{
    g_PlayerData.active = true;

    // 待機アニメーションから開始する
    StartPlayerAnimation(PLAYER_ANIM_IDLE);

}

bool CheckPlayerBlockCollision(VECTOR playerPos, int mapX, int mapY)
{
    // マップ範囲外なら衝突しない
    if (mapX < 0 || mapX >= MAP_CHIP_X_NUM ||
        mapY < 0 || mapY >= MAP_CHIP_Y_NUM)
    {
        return false;
    }

    MapChipData chip = GetMapChipData(mapX, mapY,g_WorldType);

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

            if (nextX > g_PlayerData.pos.x)
            {
                // 右方向の衝突
                return x * MAP_CHIP_WIDTH - PLAYER_HIT_WIDTH - hitX;
            }
            else if (nextX < g_PlayerData.pos.x)
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

            if (nextY > g_PlayerData.pos.y)
            {
                // 下方向の衝突
                return y * MAP_CHIP_HEIGHT - PLAYER_HIT_HEIGHT - hitY;
            }
            else if (nextY < g_PlayerData.pos.y)
            {
                // 上方向の衝突
                return (y + 1) * MAP_CHIP_HEIGHT - hitY;
            }
        }
    }

    return nextY;
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


    // X方向の移動先
    float afterX = g_PlayerData.pos.x + g_PlayerData.moveX;

    afterX = CheckPlayerCollisionX(afterX, g_PlayerData.pos.y);

    g_PlayerData.pos.x = afterX;

    // ジャンプ
    if (IsInputKey(KEY_INPUT_SPACE) &&
        !g_PlayerData.isAir)
    {
        g_PlayerData.moveY = -12.0f;
        g_PlayerData.isAir = true;
    }

    // 重力
    g_PlayerData.moveY += 0.5f;

    float beforeY = g_PlayerData.pos.y;
    float nextY = beforeY + g_PlayerData.moveY;

    float afterY = CheckPlayerCollisionY(
        nextY, g_PlayerData.pos.x
    );

    bool onGround = false;

    // 落下中で、かつ実際に上に押し戻された（または床にぶつかった）場合
    if (g_PlayerData.moveY > 0.0f && afterY != nextY)
    {
        onGround = true;
        g_PlayerData.moveY = 0.0f;
    }
    else if (afterY < nextY)
    {
        g_PlayerData.moveY = 0.0f;
    }
    else if (afterY > nextY)
    {
        // 上方向の衝突
        g_PlayerData.moveY = 0.0f;
    }

    // 衝突前の空中状態を保存
    bool wasAir = g_PlayerData.isAir;

    g_PlayerData.pos.y = afterY;
    g_PlayerData.isAir = !onGround;

    if (wasAir && onGround)
    {
        StartPlayerAnimation(PLAYER_ANIM_LANDING);
    }

    //攻撃
    if (IsTriggerKey(KEY_INPUT_Z) &&
        !g_PlayerData.isAttack &&
        g_PlayerData.attackcoolTimer <= 0)
    {
        g_PlayerData.isAttack = true;
        StartPlayerAnimation(PLAYER_ANIM_ATTACK);

        g_PlayerData.isAttackHit = false;
        g_PlayerData.attackTimer = 6;
        g_PlayerData.attackcoolTimer = 20;
    }

    //攻撃タイマー
    if (g_PlayerData.attackTimer > 0)
    {
        g_PlayerData.attackTimer--;

        if (g_PlayerData.attackTimer <= 0)
        {
            g_PlayerData.isAttack = false;
        }
    }

    //攻撃クールタイム
    if (g_PlayerData.attackcoolTimer > 0)
    {
        g_PlayerData.attackcoolTimer--;
    }
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

    CameraData camera = GetCamera();

    /// 当たり判定を可視化
    float hitX = g_PlayerData.pos.x +
        (PLAYER_WIDTH - PLAYER_HIT_WIDTH) / 2.0f;

    float hitY = g_PlayerData.pos.y + 8.0f +
        (PLAYER_HEIGHT - PLAYER_HIT_HEIGHT) / 2.0f;

    DrawBox(
        (int)hitX - camera.pos.x,
        (int)hitY - camera.pos.y,
        (int)(hitX + PLAYER_HIT_WIDTH - camera.pos.x),
        (int)(hitY + PLAYER_HIT_HEIGHT - camera.pos.y),
        GetColor(255, 0, 0),
        FALSE
    );

    

    // アニメーション描画でプレイヤーを描画する
    if (g_PlayerData.playAnim == PLAYER_ANIM_NONE)
    {
        return;
    }

    PlayerAnimationType animType = g_PlayerData.playAnim;
    AnimationData* animData = &g_PlayerData.animation[animType];

    int drawhandle = animData->handle;

    if (g_WorldType == WORLD_SHADOW)
    {
        drawhandle = animData->invertedhandle;
    }

    float drawX = g_PlayerData.pos.x +
        (PLAYER_WIDTH - animData->frameWidth) / 2.0f;

    DrawAnimation(animData, drawX - camera.pos.x ,g_PlayerData.pos.y - camera.pos.y + 8.0f , g_PlayerData.isTurn);

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

    if (anim == PLAYER_ANIM_LANDING)
    {
        g_PlayerData.animation[PLAYER_ANIM_LANDING].timer = 0;
    }

    // 再生に必要なデータを取得
    AnimationData* animData = &g_PlayerData.animation[anim];

    // フレームをリセット
    animData->nowFrame = 0;
    animData->timer = 0;

    PlayerAnimationParam animParam = PLAYER_ANIM_PARAM[anim];

    // アニメーション再生
    StartAnimation(
        animData,
        g_PlayerData.pos.x,
        g_PlayerData.pos.y,
        animParam.interval,
        animParam.frameNum,
        animParam.width,
        animParam.height,
        anim != PLAYER_ANIM_LANDING &&
        anim != PLAYER_ANIM_ATTACK
    );
}

void UpdatePlayerAnimation()
{

    // 攻撃アニメーション中
    if (g_PlayerData.isAttack)
    {
        AnimationData* animData =
            &g_PlayerData.animation[PLAYER_ANIM_ATTACK];

        if (animData->active)
        {
            UpdateAnimation(animData);
        }

        return;
    }

    // 着地アニメーション中
    if (g_PlayerData.playAnim == PLAYER_ANIM_LANDING)
    {
        AnimationData* animData =
            &g_PlayerData.animation[PLAYER_ANIM_LANDING];

        if (animData->active)
        {
            UpdateAnimation(animData);
            return;
        }

        // 着地アニメーション終了
        g_PlayerData.playAnim = PLAYER_ANIM_NONE;
    }

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

    if (animType != PLAYER_ANIM_NONE)
    {
        AnimationData* animData = &g_PlayerData.animation[animType];

        UpdateAnimation(animData);
    }
}