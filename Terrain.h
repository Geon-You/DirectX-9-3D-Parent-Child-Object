#pragma once
#include "Define.h"

typedef class TerrainVertex
{
public:
	D3DXVECTOR3 p; // 포인트
	D3DXVECTOR3 n; // 노멀 (법선)
	D3DXVECTOR3 t; // 탄젠트
}TV;

class MyIndex
{
public:
	WORD _0, _1, _2;
};

class Terrain
{
private:
	LPDIRECT3DVERTEXBUFFER9 vb = 0; // 정점을 보관할 버퍼
	LPDIRECT3DINDEXBUFFER9 ib = 0; // 인덱스를 보관할 버퍼
	LPDIRECT3DTEXTURE9 texDiffuse = 0; // 텍스쳐 색상맵
	LPDIRECT3DTEXTURE9 texHeight = 0; // 텍스쳐 높이맵

	DWORD cxHeight; // 텍스쳐 가로 크기
	DWORD czHeight; // 텍스쳐 세로 크기

	D3DXMATRIXA16 matRot; // 터레인 회전에 사용할 행렬

public:
	void Initialize();
	void Update();
	void Render();
	void Release();

	HRESULT InitTexture(); // 텍스쳐 로드 및 초기화
	HRESULT InitVB(); // 정점 버퍼 초기화
	HRESULT InitIB(); // 인덱스 버퍼 초기화

	void DrawMesh();
	void Animate(); // 회전 애니메이션
};

