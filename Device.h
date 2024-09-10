#pragma once
#include "Include.h"

class Device
{
public:
	DECLARE_SINGLETON(Device);

private:
	Device();
public:
	~Device();

private:
	// 장치 성능 조사를 위한 다이렉트 객체를 담을 필드
	LPDIRECT3D9 direct;
	// 장치를 대표하는 객체
	LPDIRECT3DDEVICE9 device;
	// 이미지 렌더를 대표하는 객체
	LPD3DXSPRITE sprite;
	// 폰트
	LPD3DXFONT font;

public:
	HRESULT InitDevice(WinMode mode); // 장치초기화
	void RenderBegin(); // 그리기 시작 전, 호출
	void RenderEnd();   // 그리기 끝난 후, 호출
	void Release();

public:
	LPDIRECT3DDEVICE9 GetDevice() { return device; }
	LPD3DXSPRITE GetSprite() { return sprite; }
	LPD3DXFONT GetFont() { return font; }
};

