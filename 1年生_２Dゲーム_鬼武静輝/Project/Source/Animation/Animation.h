
#pragma once

// アニメーションデータ
struct AnimationData
{
    bool active;       // 再生中かどうか
    bool isLoop;       // ループ再生するか

    int handle;        // 画像ハンドル
    int nowFrame;      // 現在のコマ番号
    int frameNum;      // 全コマ数

    int frameWidth;    // 1コマの幅
    int frameHeight;   // 1コマの高さ

    int timer;         // コマ送り用タイマー
    int interval;      // コマ送りの間隔

    float posX;        // 描画位置X
    float posY;        // 描画位置Y
};

// 初期化
void InitAnimation(AnimationData* animData);

// アニメーション開始
void StartAnimation(
    AnimationData* animData,
    float posX,
    float posY,
    int interval,
    int frameNum,
    int width,
    int height,
    bool isLoop
);

// アニメーション更新
void UpdateAnimation(AnimationData* animData);

// アニメーション描画
void DrawAnimation(
    AnimationData* animData,
    bool isTurnX = false,
    bool isTurnY = false
);

// 座標を指定してアニメーション描画
void DrawAnimation(
    AnimationData* animData,
    float posX,
    float posY,
    bool isTurnX = false,
    bool isTurnY = false
);
