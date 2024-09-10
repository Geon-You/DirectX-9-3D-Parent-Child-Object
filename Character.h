#pragma once
#include "Include.h"

class Character
{
private:
	LPD3DXMESH mesh; // 메시를 담을 필드
	// 재질이란? 모델이 출력하는데 필요한 데이터를 갖음
	D3DMATERIAL9* meshMat; // 메시에서 사용할 재질
	LPDIRECT3DTEXTURE9* meshTex; // 메시에서 사용할 텍스쳐
	DWORD matNum; // 메시에서 사용 중인 재질의 개수
	D3DXVECTOR3 pos;

public:
	void Initialize();
	void Update();
	void Render();
	void Release();
};

