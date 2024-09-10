#include "Device.h"

Device::Device()
	: direct(nullptr), 
	device(nullptr), 
	sprite(nullptr),
	font(nullptr)
{
}

Device::~Device()
{
	Release();
}

HRESULT Device::InitDevice(WinMode mode)
{
	// 1. 다이렉트 객체 생성
	// 2. 다이렉트 객체를 통한 장치 성능 조사
	// 3. 조사한 장치 정보를 토대로 렌더 처리에 대한 각종 설정
	// 4. 설정값을 토대로 최종적인 디바이스 장치 생성

	// 다이렉트 객체 생성
	direct = Direct3DCreate9(D3D_SDK_VERSION);

	// 장치 성능에 대한 정보를 담을 구조체
	D3DCAPS9 deviceCaps;
	ZeroMemory(&deviceCaps, sizeof(deviceCaps));

	// 장치 성능 조사
	if (FAILED(direct->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &deviceCaps)))
	{
		ERR_MSG(g_hWnd, L"장치 정보 얻어오기 실패");
		return E_FAIL;
	}

	DWORD behaivorFlag = 0;
	// 하드웨어 버텍스 프로세싱 지원 여부 확인
	if (deviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		// 지원하다면 하드웨어 버텍스 프로세싱 사용
		behaivorFlag = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		// 아니라면 소프트웨어 버텍스 프로세싱 사용
		behaivorFlag = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	// 서피스 (Surface)
	// 1. 지오메트리 or 이미지 처리에 의해 생성된 픽셀 (컬러버퍼 -> 전면/후면)
	// 2. 깊이/스텐실 정보 (깊이/스텐실 버퍼)

	// 프론트(전면) 버퍼
	// 현재 비디오 장치에 출력되고 있는 메모리 영역 (컬러 버퍼)
	// 
	// 백(후면) 버퍼
	// 출력 속도 향상을 프론트 버퍼와 동일한 색상 정보를 저장하고 있는 영역
	// 컬러 버퍼->  픽셀 저장
	// 깊이 버퍼->  새로운 픽셀을 컬러 버퍼에 덮어쓸 것인가에 대한 판별 값
	//				(기준 z 값,)
	// 스텐실 버퍼-> 깊이 버퍼와 비슷, 특정 영역을 그릴지에 대한 여부 (기준 masking 값)

	// 렌더에 사용할 전면/후면 및 깊이/스텐실 버퍼에 대한 설정 값을 담을 구조체
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	// 후면 버퍼 설정
	d3dpp.BackBufferWidth = WIN_SIZE_X;
	d3dpp.BackBufferHeight = WIN_SIZE_Y;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

	// 깊이 및 스텐실 버퍼 설정
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.EnableAutoDepthStencil = true;

	// 최종적으로 렌더할 윈도우의 핸들을 설정
	d3dpp.hDeviceWindow = g_hWnd;
	// 윈도우 모드 (창, 풀모드)
	d3dpp.Windowed = mode;

	// 스왑 체인 -> 복수의 서피스를 하나의 컬렉션으로 관리
	
	// 보여지는 곳      안 보이는 곳
	// 서피스1			서피스2
	// 전면 버퍼        후면 버퍼
	// 서피스2          서피스1
	// 전면 버퍼        후면 버퍼

	// 스왑 체인 (더블 버퍼링) 교환 방식을 설정
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	
	// 화면 재생률 설정 (화면을 새로 고치는 속도)
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	// 후면 버퍼를 전면 버퍼에 표시할 수 있는 최대속도 설정
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	// 장치 생성
	if (FAILED(direct->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		g_hWnd, behaivorFlag, &d3dpp, &device)))
	{
		ERR_MSG(g_hWnd, L"장치 생성 실패");
		return E_FAIL;
	}

	// 폰트 정보 설정
	D3DXFONT_DESC fontInfo;
	ZeroMemory(&fontInfo, sizeof(fontInfo));

	fontInfo.Height = 20;
	fontInfo.Width = 10;
	fontInfo.Weight = FW_HEAVY;
	fontInfo.CharSet = HANGUL_CHARSET;
	lstrcpy(fontInfo.FaceName, L"궁서");

	// 폰트 객체 생성
	if (FAILED(D3DXCreateFontIndirect(device, &fontInfo, &font)))
	{
		ERR_MSG(g_hWnd, L"폰트 생성 실패");
		return E_FAIL;
	}

	return S_OK;
}

// 1. 화면을 Clear() 함수로 초기화
// 2. 장면을 후면 버퍼에 그리기 위해 device의 BeginScene() 을 호출
// 3. 장면을 그린다. (내가 원하는 것을 그리고)
// 4. 다 그렸으면 EndScene() 함수를 호출
// 5. Present() 함수로 전면이랑 후면을 교체
// 6. 1~5 반복

// 디바이스 그리기 시작
// 씬 렌더
// 디바이스 그리기 끝


void Device::RenderBegin()
{
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);
	device->BeginScene();
}

void Device::RenderEnd()
{
	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
}

void Device::Release()
{
	if (sprite)
		sprite->Release();
	
	if (device)
		device->Release();
	
	if (direct)
		direct->Release();
}
