#pragma once
#include "Include.h"

class BillBoard
{
private:
	LPDIRECT3DTEXTURE9 tex;
	IDirect3DVertexBuffer9* vb;

public:
	void Initialize();
	void Update();
	void Render();
	void Release();
};