#include "Shape.h"
#include "Device.h"

void Shape::Initialize()
{
	auto device = GET_SINGLE(Device)->GetDevice();

	// 광원 초기화
	light = D3DXVECTOR3(0, 0, 0);

	// 충돌 박스 설정
	// 버텍스와 달리, 큐브 모양의 충돌체를 표현하는데 필요한 최소한의 점 8개만 있으면 됨
	center = new D3DXVECTOR3[8]
	{
		D3DXVECTOR3(-5, 5, -5), D3DXVECTOR3(5, 5, -5), D3DXVECTOR3(-5, 5, 5),
		D3DXVECTOR3(5, 5, 5), D3DXVECTOR3(-5, -5, -5), D3DXVECTOR3(5, -5, -5),
		D3DXVECTOR3(-5, -5, 5), D3DXVECTOR3(5, -5, 5)
	};

	// 텍스쳐 로딩 (벽면 텍스쳐)
	if (FAILED(D3DXCreateTextureFromFile(device, L"stone.bmp", &texture)))
	{
		// 텍스쳐 생성 실패 시, 메세지 팝업
		MessageBox(NULL, L"Texture Load Failed", L"Fail", MB_OK);
		return;
	}

	// 텍스쳐 로딩 (노멀맵 텍스쳐)
	if (FAILED(D3DXCreateTextureFromFile(device, L"stoneNormal.bmp", &textureNormal)))
	{
		// 텍스쳐 생성 실패 시, 메세지 팝업
		MessageBox(NULL, L"Texture Load Failed", L"Fail", MB_OK);
		return;
	}

	// 텍스쳐 필터링 적용
	// 텍스쳐가 축소 됐을 때, 샘플링할 방법 설정
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	// 텍스쳐가 확대 됐을 때, 샘플링할 방법 설정
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// 텍스쳐 필터링 적용
	// 텍스쳐가 축소 됐을 때, 샘플링할 방법 설정
	device->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	// 텍스쳐가 확대 됐을 때, 샘플링할 방법 설정
	device->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// 0번 텍스쳐 인덱스 사용
	device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	// 1번 텍스쳐 인덱스 사용
	device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);


	// 버텍스 버퍼 생성
	if (FAILED(device->CreateVertexBuffer(sizeof(VERTEX) * 24, NULL,
		D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &vb, NULL)))
	{
		// 버텍스 버퍼 생성 실패 시, 메세지 팝업
		MessageBox(NULL, L"Create VB Failed", L"Fail", MB_OK);
		return;
	}

	// 버텍스 버퍼 잠금 (읽고 쓰기는 동시에 진행될 수 없으므로)
	VERTEX* vertice;
	if (FAILED(vb->Lock(0, sizeof(VERTEX) * 24, (void**)&vertice, NULL)))
		return;

	// 수정
	// 1번 면
	vertice[0].position = D3DXVECTOR3(-5, 5, -5);
	vertice[0].color = 0xFFFFFFFF;
	vertice[0].tu = 0;
	vertice[0].tv = 0;

	vertice[1].position = D3DXVECTOR3(5, 5, -5);
	vertice[1].color = 0xFFFFFFFF;
	vertice[1].tu = .33f;
	vertice[1].tv = 0;

	vertice[2].position = D3DXVECTOR3(-5, -5, -5);
	vertice[2].color = 0xFFFFFFFF;
	vertice[2].tu = 0;
	vertice[2].tv = .5f;

	vertice[3].position = D3DXVECTOR3(5, -5, -5);
	vertice[3].color = 0xFFFFFFFF;
	vertice[3].tu = .33f;
	vertice[3].tv = .5f;

	// 2번 면
	vertice[4].position = D3DXVECTOR3(5, 5, 5);
	vertice[4].color = 0xFFFFFFFF;
	vertice[4].tu = .66f;
	vertice[4].tv = .5f;

	vertice[5].position = D3DXVECTOR3(-5, 5, 5);
	vertice[5].color = 0xFFFFFFFF;
	vertice[5].tu = 1;
	vertice[5].tv = .5f;

	vertice[6].position = D3DXVECTOR3(5, -5, 5);
	vertice[6].color = 0xFFFFFFFF;
	vertice[6].tu = .66f;
	vertice[6].tv = 1;

	vertice[7].position = D3DXVECTOR3(-5, -5, 5);
	vertice[7].color = 0xFFFFFFFF;
	vertice[7].tu = 1;
	vertice[7].tv = 1;

	// 3번 면
	vertice[8].position = D3DXVECTOR3(-5, 5, 5);
	vertice[8].color = 0xFFFFFFFF;
	vertice[8].tu = .33f;
	vertice[8].tv = 0;

	vertice[9].position = D3DXVECTOR3(5, 5, 5);
	vertice[9].color = 0xFFFFFFFF;
	vertice[9].tu = .66f;
	vertice[9].tv = 0;

	vertice[10].position = D3DXVECTOR3(-5, 5, -5);
	vertice[10].color = 0xFFFFFFFF;
	vertice[10].tu = .33f;
	vertice[10].tv = .5f;

	vertice[11].position = D3DXVECTOR3(5, 5, -5);
	vertice[11].color = 0xFFFFFFFF;
	vertice[11].tu = .66f;
	vertice[11].tv = .5f;

	// 4번 면
	vertice[12].position = D3DXVECTOR3(5, 5, -5);
	vertice[12].color = 0xFFFFFFFF;
	vertice[12].tu = .66f;
	vertice[12].tv = 0;

	vertice[13].position = D3DXVECTOR3(5, 5, 5);
	vertice[13].color = 0xFFFFFFFF;
	vertice[13].tu = 1;
	vertice[13].tv = 0;

	vertice[14].position = D3DXVECTOR3(5, -5, -5);
	vertice[14].color = 0xFFFFFFFF;
	vertice[14].tu = .66f;
	vertice[14].tv = .5f;

	vertice[15].position = D3DXVECTOR3(5, -5, 5);
	vertice[15].color = 0xFFFFFFFF;
	vertice[15].tu = 1;
	vertice[15].tv = .5f;

	// 5번 면
	vertice[16].position = D3DXVECTOR3(-5, -5, -5);
	vertice[16].color = 0xFFFFFFFF;
	vertice[16].tu = 0;
	vertice[16].tv = .5f;

	vertice[17].position = D3DXVECTOR3(5, -5, -5);
	vertice[17].color = 0xFFFFFFFF;
	vertice[17].tu = .33f;
	vertice[17].tv = .5f;

	vertice[18].position = D3DXVECTOR3(-5, -5, 5);
	vertice[18].color = 0xFFFFFFFF;
	vertice[18].tu = 0;
	vertice[18].tv = 1;

	vertice[19].position = D3DXVECTOR3(5, -5, 5);
	vertice[19].color = 0xFFFFFFFF;
	vertice[19].tu = .33f;
	vertice[19].tv = 1;

	// 6번 면
	vertice[20].position = D3DXVECTOR3(-5, 5, 5);
	vertice[20].color = 0xFFFFFFFF;
	vertice[20].tu = .33f;
	vertice[20].tv = .5f;

	vertice[21].position = D3DXVECTOR3(-5, 5, -5);
	vertice[21].color = 0xFFFFFFFF;
	vertice[21].tu = .66f;
	vertice[21].tv = .5f;

	vertice[22].position = D3DXVECTOR3(-5, -5, 5);
	vertice[22].color = 0xFFFFFFFF;
	vertice[22].tu = .33f;
	vertice[22].tv = 1;

	vertice[23].position = D3DXVECTOR3(-5, -5, -5);
	vertice[23].color = 0xFFFFFFFF;
	vertice[23].tu = .66f;
	vertice[23].tv = 1;

	// 수정이 끝난 후, 버퍼 잠금 해제
	vb->Unlock();

	shapeNum = 1;
	pos = D3DXVECTOR3(0, 0, 0);
	scale = D3DXVECTOR3(1.f, 1.f, 1.f);
	angle = D3DXVECTOR3(0, 0, 0);
}

void Shape::Update()
{
	// 그릴 메시 설정
	if (GetAsyncKeyState('1')) shapeNum = 1;
	if (GetAsyncKeyState('2')) shapeNum = 2;
	if (GetAsyncKeyState('3')) shapeNum = 3;
	if (GetAsyncKeyState('4')) shapeNum = 4;

	// x축 회전
	if (GetAsyncKeyState('Q')) angle.x -= 1.f;
	if (GetAsyncKeyState('W')) angle.x += 1.f;

	// y축 회전
	if (GetAsyncKeyState('E')) angle.y -= 1.f;
	if (GetAsyncKeyState('R')) angle.y += 1.f;

	// z축 회전
	if (GetAsyncKeyState('T')) angle.z -= 1.f;
	if (GetAsyncKeyState('Y')) angle.z += 1.f;

	// 이동
	if (GetAsyncKeyState(VK_NUMPAD4)) pos.x -= 1.f;
	if (GetAsyncKeyState(VK_NUMPAD6)) pos.x += 1.f;

	if (GetAsyncKeyState(VK_NUMPAD8)) pos.y -= 1.f;
	if (GetAsyncKeyState(VK_NUMPAD2)) pos.y += 1.f;

	if (GetAsyncKeyState(VK_NUMPAD7)) pos.z -= 1.f;
	if (GetAsyncKeyState(VK_NUMPAD9)) pos.z += 1.f;
}

void Shape::Render()
{
	if (isPicking)
		return;

	CalculateLight();

	auto device = GET_SINGLE(Device)->GetDevice();

	// 광원 설정 (기본으로 광원이 켜져있음) -> 광원을 끌 거임
	// -> 광원 옵션은 켜져있지만, 실제 월드에 배치된 광원이 없으므로 광원을 끌 예정
	// 광원을 끌 경우, 메시의 버텍스가 가지고 있는 기본 색상이 나옴
	device->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 그리기 상태 설정
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	// 백스페이스 컬링 (뒷면 추려내기, 최적화 기법)
	// 백스페이스 컬링 -> 시계방향(CW) or 반시계방향(CCW)
	// 컬링 기본 설정 값은 CCW, 뒷면은 그리지 않는다.
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	D3DXMATRIX matTrans; // 위치 행렬
	D3DXMATRIX matRot;	 // 회전 행렬
	D3DXMATRIX matScale; // 크기 행렬
	D3DXMATRIX matWorld; // 월드 행렬

	// 행렬 연산으로 여러 축의 회전을 표현 시,
	// 짐벌락 문제가 발생함
	// 이 짐벌락 문제를 해결 할 수 있는 것이 쿼터니언
	// 그리고 여러 축에 대한 회전 연산에 대해서는 쿼터니언이 연산속도가 더 빠름 (단일 축에 대한 계산은 행렬 연산이 더 빠름)
	// 행렬연산은 degree나 radian을 이용한 연산(직관적)을 하지만 쿼터니언은 우리가 직관적으로 알아볼 수 있는 연산이 아니다(수학적인 지식이 필요함).

	D3DXQUATERNION q; // 쿼터니언
	D3DXVECTOR3 radAngle; // 라디안으로 변환한 앵글 값을 가질 벡터

	radAngle.x = D3DXToRadian(angle.x);
	radAngle.y = D3DXToRadian(angle.y);
	radAngle.z = D3DXToRadian(angle.z);

	// 위치 행렬 변환
	D3DXMatrixTranslation(&matTrans, pos.x, pos.y, pos.z);

	// 회전 행렬 변환
	D3DXQuaternionRotationYawPitchRoll(&q, radAngle.y, radAngle.x, radAngle.z);
	D3DXMatrixRotationQuaternion(&matRot, &q);

	// 크기 행렬 변환
	D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z);

	// 월드 행렬 연산
	matWorld = matScale * matRot * matTrans;
	// 연산한 월드 행렬을 적용
	device->SetTransform(D3DTS_WORLD, &matWorld);

	// 광원 벡터를 RGBA로 변환
	DWORD dwFactor = VectorToRGBA(&light);
	// RGBA로 변환된 벡터를 TextureFactor 값으로 등록
	device->SetRenderState(D3DRS_TEXTUREFACTOR, dwFactor);

	// 노멀맵 텍스쳐의 RGBA와 광원 벡터를 내적
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DOTPRODUCT3);

	// 노멀맵 텍스쳐와 법선맵 텍스쳐를 섞어서 출력
	device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
	device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
	

	// 텍스쳐 설정 (텍스쳐 매핑)
	device->SetTexture(0, textureNormal);
	device->SetTexture(1, texture);

	// 출력할 버텍스 버퍼 설정
	device->SetStreamSource(0, vb, 0, sizeof(VERTEX));
	// FVF 설정
	device->SetFVF(D3DFVF_CUSTOMVERTEX);

	// 삼각형 2개를 이용하여 한 면을 그림
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, 2);
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, 2);
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, 2);
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 20, 2);
}

void Shape::Release()
{
	if (texture)
	{
		texture->Release();
		texture = nullptr;
	}

	if (textureNormal)
	{
		textureNormal->Release();
		textureNormal = nullptr;
	}

	if (vb)
	{
		vb->Release();
		vb = nullptr;
	}

	if (center)
	{
		delete center;
		center = nullptr;
	}
}

void Shape::CalculateLight()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	light.x = -(((2.f * pt.x) / WIN_SIZE_X) - 1);
	light.y = -(((2.f * pt.y) / WIN_SIZE_Y) - 1);
	light.z = 0;

	if (D3DXVec3Length(&light) > 1.f)
	{
		D3DXVec3Normalize(&light, &light);
	}
	else
	{
		light.z = sqrt(1.f - light.x * light.x - light.y * light.y);
	}
}

DWORD Shape::VectorToRGBA(D3DXVECTOR3* v)
{
	DWORD r = (DWORD)(127.f * v->x + 128.f);
	DWORD g = (DWORD)(127.f * v->y + 128.f);
	DWORD b = (DWORD)(127.f * v->z + 128.f);
	DWORD a = (DWORD)255.f;

	return (a << 24) + (r << 16) + (g << 8) + b;
}

bool Shape::OncollisionEnter(D3DXVECTOR3* ori, D3DXVECTOR3* dir)
{
	// 주사위와 레이의 충돌 정보를 판별
	// 레이와 메시의 삼각형 충돌 시, 충돌 지점의 x, y, 거리 t(z축)
	float u, v, t;
	isPicking = false;

	// 삼각형의 각 버텍스 위치를 담을 변수
	D3DXVECTOR3 v1, v2, v3;
	v1 = center[0] + pos;
	v2 = center[1] + pos;
	v3 = center[2] + pos;

	// 광선과 삼각형의 충돌 체크 -> D3DXIntersectTri();
	if (D3DXIntersectTri(&v1, &v2, &v3, ori, dir, &u, &v, &t))
	{
		isPicking = true;
		return true;
	}

	v1 = center[1] + pos;
	v2 = center[2] + pos;
	v3 = center[3] + pos;

	if (D3DXIntersectTri(&v1, &v2, &v3, ori, dir, &u, &v, &t))
	{
		isPicking = true;
		return true;
	}

	v1 = center[0] + pos;
	v2 = center[1] + pos;
	v3 = center[4] + pos;

	if (D3DXIntersectTri(&v1, &v2, &v3, ori, dir, &u, &v, &t))
	{
		isPicking = true;
		return true;
	}

	v1 = center[0] + pos;
	v2 = center[1] + pos;
	v3 = center[5] + pos;

	if (D3DXIntersectTri(&v1, &v2, &v3, ori, dir, &u, &v, &t))
	{
		isPicking = true;
		return true;
	}

	v1 = center[1] + pos;
	v2 = center[5] + pos;
	v3 = center[7] + pos;

	if (D3DXIntersectTri(&v1, &v2, &v3, ori, dir, &u, &v, &t))
	{
		isPicking = true;
		return true;
	}

	v1 = center[1] + pos;
	v2 = center[7] + pos;
	v3 = center[3] + pos;

	if (D3DXIntersectTri(&v1, &v2, &v3, ori, dir, &u, &v, &t))
	{
		isPicking = true;
		return true;
	}

	v1 = center[0] + pos;
	v2 = center[2] + pos;
	v3 = center[6] + pos;

	if (D3DXIntersectTri(&v1, &v2, &v3, ori, dir, &u, &v, &t))
	{
		isPicking = true;
		return true;
	}

	v1 = center[0] + pos;
	v2 = center[4] + pos;
	v3 = center[6] + pos;

	if (D3DXIntersectTri(&v1, &v2, &v3, ori, dir, &u, &v, &t))
	{
		isPicking = true;
		return true;
	}

	v1 = center[2] + pos;
	v2 = center[6] + pos;
	v3 = center[3] + pos;

	if (D3DXIntersectTri(&v1, &v2, &v3, ori, dir, &u, &v, &t))
	{
		isPicking = true;
		return true;
	}

	v1 = center[7] + pos;
	v2 = center[6] + pos;
	v3 = center[3] + pos;

	if (D3DXIntersectTri(&v1, &v2, &v3, ori, dir, &u, &v, &t))
	{
		isPicking = true;
		return true;
	}

	v1 = center[6] + pos;
	v2 = center[4] + pos;
	v3 = center[5] + pos;

	if (D3DXIntersectTri(&v1, &v2, &v3, ori, dir, &u, &v, &t))
	{
		isPicking = true;
		return true;
	}

	v1 = center[6] + pos;
	v2 = center[5] + pos;
	v3 = center[7] + pos;

	if (D3DXIntersectTri(&v1, &v2, &v3, ori, dir, &u, &v, &t))
	{
		isPicking = true;
		return true;
	}

	return false;
}
