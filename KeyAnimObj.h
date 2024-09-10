#pragma once
#include "Define.h"

typedef struct VERTEXANIM
{
	float x, y, z;
	DWORD color;

}VA;

struct MYINDEXANIM
{
	WORD _0, _1, _2;
};

class KeyAnimObj
{
private:
	LPDIRECT3DVERTEXBUFFER9 vb;		// 정점 버퍼
	LPDIRECT3DINDEXBUFFER9 ib;		// 인덱스 버퍼

	D3DXMATRIXA16 matTransParent;	// 부모 객체의 위치 행렬
	D3DXMATRIXA16 matRotParent;		// 부모 객체의 회전 행렬

	D3DXMATRIXA16 matTransChild;	// 자식 객체의 위치 행렬
	D3DXMATRIXA16 matRotChild;		// 자식 객체의 회전 행렬

	// 애니메이션 키 값을 보관하기 위한 배열
	D3DXVECTOR3 aniPos[2];			// 위치 키 값
	D3DXQUATERNION aniRot[2];		// 회전 키 값

public:
	void Initialize();
	void Update();
	void Render();
	void Release();

	HRESULT InitVB();				// 정점 버퍼 초기화
	HRESULT InitIB();				// 인덱스 버퍼 초기화
	HRESULT InitGeometry();			// 기하 정보 초기화

	void InitAnimation();			// 애니메이션 초기화
	void Animate();					// 애니메이션 실행
	void DrawMesh(D3DXMATRIXA16* mat); //  메시 그리기

	// 선형 보간 기능
	float Linear(float a, float b, float t);
};

