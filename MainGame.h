#pragma once
#include "Include.h"

class Scene;

class MainGame
{
private:
	Scene* scene;

public:
	void Initialize(); // 초기화
	void Update(); // 게임 내 각종 연산
	void Render(); // 게임 내 그리기
	void Release(); // 메모리 해제
	void OnMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	MainGame();
	~MainGame();
};

