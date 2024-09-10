#pragma once
#include "Include.h"

class Scene
{
public:
	virtual void Initialize() abstract;
	virtual int Update() abstract;
	virtual void Render() abstract;
	virtual void Release() abstract;
	virtual void OnMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {};

public:
	Scene();
	virtual ~Scene();
};

