#include "DxLib.h"
#include "Animation.h"

void InitAnimation(AnimationData* animData)
{
    animData->active = false;
    animData->isLoop = false;
    animData->handle = -1;
    animData->nowFrame = 0;
    animData->frameNum = 0;
    animData->frameWidth = 0;
    animData->frameHeight = 0;
    animData->timer = 0;
    animData->interval = 0;
    animData->posX = 0.0f;
    animData->posY = 0.0f;
}

void StartAnimation(AnimationData* animData,float posX,float posY,int interval,int frameNum,int width,int height,bool isLoop)
{
	// アクティブ
	animData->active = true;

	// 座標設定
	animData->posX = posX;
	animData->posY = posY;

	// インターバル設定
	animData->interval = interval;

	// タイマー設定
	animData->timer = animData->interval;

	// 0コマ目にする
	animData->nowFrame = 0;

	// コマ数
	animData->frameNum = frameNum;

	// 1コマのサイズ
	animData->frameWidth = width;
	animData->frameHeight = height;

	// ループするか
	animData->isLoop = isLoop;
}

void UpdateAnimation(AnimationData* animData)
{
    // 再生中でなければ何もしない
    if (!animData->active)
    {
        return;
    }

    // タイマーが0になったら次のコマへ進む
    if (animData->timer <= 0)
    {
        animData->nowFrame++;
        animData->timer = animData->interval;

        // 最後のコマまで再生した場合
        if (animData->nowFrame >= animData->frameNum)
        {
            if (animData->isLoop)
            {
                // ループ再生なら最初のコマへ戻る
                animData->nowFrame = 0;
            }
            else
            {
                // ループしない場合は再生終了
                animData->active = false;
                return;
            }
        }
    }

    animData->timer--;
}

void DrawAnimation(AnimationData* animData,bool isTurnX,bool isTurnY)
{
    DrawAnimation(animData,animData->posX,animData->posY,isTurnX,isTurnY);
}

void DrawAnimation(AnimationData* animData,float posX,float posY,bool isTurnX,bool isTurnY)
{
    // アニメーションが再生中でなければ描画しない
    if (!animData->active)
    {
        return;
    }

    // 現在のコマの画像上のX座標を計算
    int frameX = animData->frameWidth * animData->nowFrame;

    // 現在のコマを描画
    DrawRectGraph((int)posX,(int)posY,frameX,0,animData->frameWidth,animData->frameHeight,animData->handle,TRUE,isTurnX,isTurnY);
}
