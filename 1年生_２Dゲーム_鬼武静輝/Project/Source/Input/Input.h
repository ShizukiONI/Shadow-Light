#pragma once

//キーの状態を初期化
void InitInput();

//キーの入力を更新
void UpdateInput();

//キーを押している間、反応する
bool IsInputKey(int key);

//キーを押した瞬間だけ、反応する
bool IsTriggerKey(int key);