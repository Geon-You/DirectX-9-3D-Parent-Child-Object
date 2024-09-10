#include "Ingame.h"
#include "Include.h"
#include "Device.h"

void Ingame::Initialize()
{
	// 이전(2D에서)까지 사용했던 변환
	// 로컬(모델) 스페이스 -> 월드 변환

	// 이번에 추가적으로 진행할 변환 (3D 공간을 2D에 나타내기 위한 변환)
	// 뷰 변환 -> 프로젝션 변환
	// 뷰 변환을 통해 월드 상에 카메라를 정의하고,
	// 프로젝션 변환을 통해 카메라가 어떤 식으로 월드를 렌더할 것인지를 결정 (직교 - Orthographic, 원근 - Perspective)

	// 뷰 변환을 위해 뷰 행렬을 정의
	// 카메라의 위치를 나타내는 벡터 -> eye
	// 카메라가 바라보고 있는 방향을 나타내는 벡터 -> lookAt
	// 월드 상에서 양의 y축(위쪽)을 가리키는 크기가 1인 벡터 -> up
	// 위의 3개의 벡터를 통해 뷰 행렬을 구성함

	// up 벡터의 설명 보충
	// up 벡터를 통해 카메라의 기저 축을 만드는 작업을 함

	// 뷰 행렬 4x4
	// right.x,   right.y,   right.z,   0		<- 카메라의 기저 x축
	// up.x,	  up.y,	     up.z,	    0		<- 카메라의 기저 y축
	// forward.x, forward.y, forward.z, 0		<- 카메라의 기저 z축
	// eye.x,	  eye.y,	 eye.z,		1		<- 카메라의 위치

	// 게임에서 일반적으로 많이 사용되는 투영 방식은 2가지
	// Orthographic		<- 원근법 적용 X
	// Perspective		<- 원근법 적용 O

	// Perspective 기준으로 투영 행렬로 인해 모델들의 크기가 거리에 따라 다르게 보임
	// 1. 종횡비 (Aspect ratio) -> 장치마다 다른 종횡비에 따라 x, y 값을 조정 (일반적으론 수직 fov를 사용하고 그때의 aspective ratio = width / height)
	// 2. 시야각 (FOV : Field Of View) 설정 -> 시야각에 의해 객체를 더 적게 또는 더 많이 보게 됨
	//										  시야각에 의해서도 x, y 값을 조정
	// 3. 정규화 : x(-1 ~ 1), y(-1 ~ 1), z(-1 ~ 1) 좌표들을 정규화시켜, NDC(정규화된 장치 공간, 카메라에 의해 그려지는 육면체 프러스텀 공간)를 얻음

	// 뷰포트 (viewport)
	// 장치에 의존적인 좌표 체계로 변환
	// 기본적으로 클라이언트 해상도로 설정

	// 카메라의 위치
	eye = D3DXVECTOR3(0, 0, -10.f);

	// 카메라가 보는 지점
	lookAt = D3DXVECTOR3(0, 0, 0);

	// 카메라의 상향 벡터
	up = D3DXVECTOR3(0, 1, 0);

	// 외적을 통해 카메라의 기저 벡터를 만들어 냄
	// 뷰 행렬 연산 (왼손좌표계 기준)
	D3DXMatrixLookAtLH(&view, &eye, &lookAt, &up);
	// 연산한 뷰 행렬을 적용
	auto device = GET_SINGLE(Device)->GetDevice();
	device->SetTransform(D3DTS_VIEW, &view);

	// 투영 행렬
	D3DXMatrixPerspectiveFovLH(&projection, D3DX_PI * .5f, 1, 1, 1000);
	// 연산한 투영 행렬을 장치에 적용
	device->SetTransform(D3DTS_PROJECTION, &projection);

	RECT rc;
	GetClientRect(g_hWnd, &rc);

	// 클라이언트 해상도를 기반으로 한 뷰포트 생성
	D3DVIEWPORT9 vp = { 0, 0, rc.right - rc.left, rc.bottom - rc.top, 0, 1 };
	// 생성한 뷰포트 적용
	device->SetViewport(&vp);

	//terrain.Initialize();
	//character.Initialize();
	//hpBar.Initialize();

	keyAnimObj.Initialize();
}

int Ingame::Update()
{
	auto device = GET_SINGLE(Device)->GetDevice();

	float zDir = 0.f;

	// W, A, S, D 이동
	if (GetAsyncKeyState('W'))
	{
		zDir = 1.f;

	}

	if (GetAsyncKeyState('S'))
	{
		zDir = -1.f;
	}

	// 카메라가 바라보는 좌표 값에서 카메라의 위치를 뺀 값을 dir에 저장
	D3DXVECTOR3 lookDir = lookAt - eye;
	// 이후, 정규화
	D3DXVec3Normalize(&lookDir, &lookDir);
	// 위의 과정을 통해 바라보는 방향 벡터를 얻었음

	eye += lookDir * 2.f * zDir;
	lookAt += lookDir * 2.f * zDir;

	// 카메라 시점은 그대로 둔 채 옆으로 이동하는 방식
	// 카메라가 보는 방향과 카메라의 상향 벡터의 수직이 되는 벡터를 얻은 후 (카메라의 x축 기저를 얻음)
	// 속력을 곱한 후, 이동하면 됨

	float xDir = 0.f;

	if (GetAsyncKeyState('A'))
	{
		xDir = -1.f;
	}

	if (GetAsyncKeyState('D'))
	{
		xDir = 1.f;
	}

	D3DXVECTOR3 cross;
	// 외적을 통해 수직이 되는 벡터를 구함
	D3DXVec3Cross(&cross, &up, &lookDir);
	D3DXVec3Normalize(&cross, &cross);

	eye += cross * 2.f * xDir;
	lookAt += cross * 2.f * xDir;

	// 카메라 y축 회전 Q, E
	if (GetAsyncKeyState('Q'))
	{
		lookAt.x -= 10.f;
	}

	if (GetAsyncKeyState('E'))
	{
		lookAt.x += 10.f;
	}

	// 변경된 내용을 뷰 행렬에 적용
	D3DXMatrixLookAtLH(&view, &eye, &lookAt, &up);
	device->SetTransform(D3DTS_VIEW, &view);

	//terrain.Update();
	//character.Update();
	//hpBar.Update();

	keyAnimObj.Update();

	return 0;
}

void Ingame::Render()
{
	//terrain.Render();
	//character.Render();
	//hpBar.Render();

	keyAnimObj.Render();

}

void Ingame::Release()
{
	//terrain.Release();
	//character.Release();
	//hpBar.Release();

	keyAnimObj.Release();
}

void Ingame::OnMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// 투영 창의 좌표로 변환할 마우스 좌표를 담을 변수
	float pointX;
	float pointY;

	auto device = GET_SINGLE(Device)->GetDevice();

	// 로컬 -> 월드 -> 뷰 -> 투영 -> 뷰포트 순으로 연산

	// 뷰포트 정보 가져오기
	D3DVIEWPORT9 vp;
	device->GetViewport(&vp);

	// 프로젝션(투영) 행렬 정보 가져오기
	D3DXMATRIX proj;
	device->GetTransform(D3DTS_PROJECTION, &proj);

	// 마우스 포인트를 투영 창의 좌표로 변환
	// -> 마우스 포인트 x, y 좌표와 화면 해상도 사이의 비율을 구해
	//	  마우스 좌표를 -1 ~ 1 사이의 값을 바꾼다.
	// -> 이후 각각의 x, y 좌표와 투영 행렬의 x, y 스케일링 요소의 비율을 구해
	//    최종적으로 투영 창의 좌표로 변환

	//				-1 ~ 1 로의 정규화 작업			   투영 행렬의 요소
	pointX = (2.f * LOWORD(lParam) / vp.Width - 1.f) / proj(0, 0);
	pointY = (-2.f * HIWORD(lParam) / vp.Height + 1.f) / proj(1, 1);

	// 변환된 마우스 포인트를 이용해서 레이 생성
	Ray ray;
	ray.origin = D3DXVECTOR3(0, 0, 0);
	ray.dir = D3DXVECTOR3(pointX, pointY, 1.f);
	
	// 레이를 월드 좌표로 역산 (마우스 좌표는 현재 투영 단계 좌표, 모델은 월드 단계의 좌표)

	// 뷰 매트릭스의 역행렬
	D3DXMATRIX viewInverse;
	D3DXMatrixInverse(&viewInverse, 0, &view);

	// 레이가 갖는 벡터를 월드 스페이스 좌표로 변환
	D3DXVec3TransformCoord(&ray.origin, &ray.origin, &viewInverse);
	D3DXVec3TransformNormal(&ray.dir, &ray.dir, &viewInverse);
	D3DXVec3Normalize(&ray.dir, &ray.dir);
}

Ingame::Ingame()
{
}

Ingame::~Ingame()
{
    Release();
}
