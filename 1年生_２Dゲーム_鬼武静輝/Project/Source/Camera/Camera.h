#pragma once
#include "DxLib.h"

// カメラ情報
struct CameraData
{
    VECTOR pos;
};

// カメラ本体
extern CameraData g_CameraData;

// 初期化
void InitCamera();

// 更新
void StepCamera();

// カメラ情報取得
CameraData GetCamera();