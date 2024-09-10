#pragma once
#include "Include.h"

class Shape
{
public:
	int shapeNum;			// 해당 필드값으로 어떤 메시를 그릴 것인지 결정
	D3DXVECTOR3 pos;		// 위치
	D3DXVECTOR3 angle;		// 회전
	D3DXVECTOR3 scale;		// 크기

private:
	LPDIRECT3DTEXTURE9 texture;			// 벽면 텍스쳐
	LPDIRECT3DTEXTURE9 textureNormal;	// 노멀맵 텍스쳐
	D3DXVECTOR3 light;					// 광원

	IDirect3DVertexBuffer9* vb;			// 버텍스 버퍼

	bool isPicking;			// 현재 마우스로 가리키고 있는지 (마우스로 충돌하고 있는지)
	D3DXVECTOR3* center;	// 충돌 박스로 사용될 점들을 담아둘 배열

public:
	void Initialize();
	void Update();
	void Render();
	void Release();
	void CalculateLight();
	DWORD VectorToRGBA(D3DXVECTOR3* v);

	bool OncollisionEnter(D3DXVECTOR3* ori, D3DXVECTOR3* dir);
};

