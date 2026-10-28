#include "DxLib.h"
#include "Input.h"

#define INPUT_KEY_MAX 256

//現在のキー状態
char g_KeyState[INPUT_KEY_MAX];

//前回のキー状態
char g_PrevKeyState[INPUT_KEY_MAX];

void InitInput()
{
	for (int i = 0; i < INPUT_KEY_MAX; i++)
	{
		g_KeyState[i] = 0;
		g_PrevKeyState[i] = 0;
	}
}

void UpdateInput()
{
	//前回の状態を保存
	for (int i = 0; i < INPUT_KEY_MAX; i++)
	{
		g_PrevKeyState[i] = g_KeyState[i];
	}

	//現在のキーボード状態を取得
	GetHitKeyStateAll(g_KeyState);
}

bool IsInputKey(int key)
{
	if (key < 0 || key >= INPUT_KEY_MAX)
	{
		return false;
	}

	return g_KeyState[key] != 0;
}

bool IsTriggerKey(int key)
{
	if (key < 0 || key >= INPUT_KEY_MAX)
	{
		return false;
	}

	return g_KeyState[key] != 0 && g_PrevKeyState[key] == 0;
}
