#pragma once
#include "Include.h"

// 베이스로 Texture 클래스가 있고
// 파생 클래스로 SingleTexture, MultiTexture
// Single - 텍스처 한 장을 보관하고 제어
// Multi - 연관된 텍스처 여러 장을 보관하고 제어

class Texture abstract
{
public:
	Texture();
	virtual ~Texture();

public:
	virtual const TEXINFO* GetTexture(const TCHAR* key = nullptr, const int& cnt = 0) abstract;
	virtual HRESULT InsertTexture(const TCHAR* path, 
		const TCHAR* key = nullptr, const int& cnt = 0) abstract;
	virtual void Release() abstract;
};

