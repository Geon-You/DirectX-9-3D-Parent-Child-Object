#include "BillBoard.h"
#include "Device.h"

void BillBoard::Initialize()
{
	auto device = GET_SINGLE(Device)->GetDevice();

	// 텍스쳐 로딩
	if (FAILED(D3DXCreateTextureFromFile(device, L"hp_bar.bmp", &tex)))
		return;

	// 텍스쳐 필터 설정
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// 버텍스 버퍼 생성
	if (FAILED(device->CreateVertexBuffer(sizeof(VERTEX) * 4, 0,
		D3DFVF_CUSTOMVERTEX_TEX, D3DPOOL_DEFAULT, &vb, NULL)))
		return;

	// 버텍스 버퍼 정보를 복사하여 받을 변수
	VERTEX* vertice;

	// 버텍스 버퍼 잠금
	if (FAILED(vb->Lock(0, sizeof(VERTEX) * 4, (void**)&vertice, 0)))
		return;

	vertice[0].position = D3DXVECTOR3(-5, 1, -5);
	vertice[0].color = 0xffffffff;
	vertice[0].tu = 0;
	vertice[0].tv = 0;

	vertice[1].position = D3DXVECTOR3(5, 1, -5);
	vertice[1].color = 0xffffffff;
	vertice[1].tu = 1.f;
	vertice[1].tv = 0;

	vertice[2].position = D3DXVECTOR3(-5, -1, -5);
	vertice[2].color = 0xffffffff;
	vertice[2].tu = 0;
	vertice[2].tv = 1.f;

	vertice[3].position = D3DXVECTOR3(5, -1, -5);
	vertice[3].color = 0xffffffff;
	vertice[3].tu = 1.f;
	vertice[3].tv = 1.f;

	vb->Unlock();
}

void BillBoard::Update()
{
}

void BillBoard::Render()
{
	auto device = GET_SINGLE(Device)->GetDevice();

	D3DXMATRIX matTrans, matRot, matScale, matWorld;
	D3DXQUATERNION quat;

	// 위치 행렬 변환
	D3DXMatrixTranslation(&matTrans, 0, 160.f, 0);

	// 회전 행렬 변환
	D3DXQuaternionRotationYawPitchRoll(&quat, 0, 0, 0);
	D3DXMatrixRotationQuaternion(&matRot, &quat);

	// 빌보드 구현 부분
	// 카메라가 현재 Y축 회전만 하므로, 카메라의 y축 회전행렬을 가져옴
	D3DXMATRIX matView;
	device->GetTransform(D3DTS_VIEW, &matView);

	matRot._11 = matView._11;
	matRot._13 = matView._13;
	matRot._31 = matView._31;
	matRot._33 = matView._33;

	// 장치에 뷰행렬이 적용되고 있으므로, 
	// 이 상태에서 객체의 월드 행렬을 적용 시키면
	// 자동으로 월드 행렬 * 뷰 행렬 연산이 이루어짐

	// Y축 회전 행렬의 역행렬을 구해서, 적용된 Y축 회전을 무효화 시킴
	D3DXMatrixInverse(&matRot, NULL, &matRot);

	// 크기 행렬
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

	// 월드 행렬
	matWorld = matScale * matRot * matTrans;
	device->SetTransform(D3DTS_WORLD, &matWorld);

	// 텍스쳐 매핑
	device->SetTexture(0, tex);

	// 출력할 버텍스 버퍼 설정
	device->SetStreamSource(0, vb, 0, sizeof(VERTEX));

	// FVF 설정
	device->SetFVF(D3DFVF_CUSTOMVERTEX_TEX);

	// 메쉬 그리기
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

void BillBoard::Release()
{
	if (vb)
	{
		vb->Release();
		vb = nullptr;
	}

	if (tex)
	{
		tex->Release();
		tex = nullptr;
	}
}