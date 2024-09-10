#include "Terrain.h"
#include "Device.h"

void Terrain::Initialize()
{
    if (FAILED(InitTexture()))
        return;

    if (FAILED(InitVB()))
        return;

    if (FAILED(InitIB()))
        return;
}

void Terrain::Update()
{
    Animate();
}

void Terrain::Render()
{
    auto device = GET_SINGLE(Device)->GetDevice();

    device->SetRenderState(D3DRS_LIGHTING, FALSE);

    // 0번 텍스쳐 스테이지에 색상맵 텍스쳐 설정
    device->SetTexture(0, texDiffuse);
    // 0번 텍스쳐에 확대 필터 설정
    device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    // 0번 텍스쳐 인덱스 사용
    device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);

    DrawMesh();
}

void Terrain::Release()
{
    if (texDiffuse != nullptr)
    {
        texDiffuse->Release();
        texDiffuse = nullptr;
    }

    if (texHeight != nullptr)
    {
        texHeight->Release();
        texHeight = nullptr;
    }

    if (vb != nullptr)
    {
        vb->Release();
        vb = nullptr;
    }

    if (ib != nullptr)
    {
        ib->Release();
        ib = nullptr;
    }
}

HRESULT Terrain::InitTexture()
{
    auto device = GET_SINGLE(Device)->GetDevice();

    // 높이맵 텍스쳐 로드
    if (FAILED(D3DXCreateTextureFromFileEx(device, L"height.bmp", D3DX_DEFAULT,
        D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED,
        D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &texHeight)))
    {
        return E_FAIL;
    }

    // 색상맵 텍스쳐 로드
    if (FAILED(D3DXCreateTextureFromFile(device, L"tile.bmp", &texDiffuse)))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT Terrain::InitVB()
{
    D3DSURFACE_DESC sd;
    D3DLOCKED_RECT rc;

    // 텍스쳐 정보 가져오기
    texHeight->GetLevelDesc(0, &sd);
    cxHeight = sd.Width; // 텍스쳐 가로 크기
    czHeight = sd.Height; // 텍스쳐 세로 크기

    auto device = GET_SINGLE(Device)->GetDevice();

    // 버텍스 버퍼 생성
    if (FAILED(device->CreateVertexBuffer(cxHeight * czHeight * sizeof(TV),
        0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &vb, NULL)))
    {
        return E_FAIL;
    }

    // 텍스쳐 메모리 락을 하면서 텍스쳐 정보를 가져옴
    texHeight->LockRect(0, &rc, NULL, D3DLOCK_READONLY);

    // 버텍스 버퍼 락
    void* vertice;
    if (FAILED(vb->Lock(0, cxHeight * czHeight * sizeof(TV), (void**)&vertice, NULL)))
    {
        return E_FAIL;
    }

    // 터레인 버텍스 수정
    // 받아온 버텍스 버퍼의 시작 주소를 터레인 버텍스 형태로 캐스팅
    // -> 결과 - 복수의 정점을 터레인 버텍스 형태로 사용 가능
    TV* pv = (TV*)vertice;
    TV v;

    // 텍스쳐 세로 크기만큼 반복
    for (DWORD z = 0; z < czHeight; ++z)
    {
        // 텍스쳐 가로 크기만큼 반복
        for (DWORD x = 0; x < cxHeight; ++x)
        {
            // 메시 데이터를 원점에 정렬하여 x, z 좌표 설정 (y축 좌표는 높이맵에서 읽어옴)
            v.p.x = (float)x - cxHeight / 2.f;
            // 왼손 좌표계는 z축 양의 방향이 모니터 안쪽이므로 -로 반전
            v.p.z = -((float)z - czHeight / 2.f);

            // 버텍스의 y (높이) 값은 높이맵 텍스쳐의 정보를 토대로 설정
            // Bits -> 잠근 텍스쳐에 대한 전체 비트 (전체 비트의 시작 주소)
            // Pitch -> 한 행에 있는 바이트 수
            v.p.y = (*((LPDWORD)rc.pBits + x + z * (rc.Pitch / 4)) & 0x000000ff) / 5.f;

            // 노멀 벡터 설정
            v.n = v.p;
            D3DXVec3Normalize(&v.n, &v.n);

            // t는 텍스쳐 맵핑을 위한 u, v 좌표
            v.t.x = (float)x / (cxHeight - 1);
            v.t.y = (float)z / (czHeight - 1);

            // pv는 여러 개의 버텍스를 갖는 버텍스 배열 같은 형태
            // 역참조를 통해 방금 정의한 하나의 버텍스 정보를 버텍스 배열의 원소에 대입한 후
            // 다음 원소로 넘어가기 위해 주소를 후처리로 ++ 시킴
            *pv++ = v;
        }
    }

    // 버텍스 버퍼, 텍스쳐 락 해제
    vb->Unlock();
    texHeight->UnlockRect(0);

    return S_OK;
}

HRESULT Terrain::InitIB()
{
    auto device = GET_SINGLE(Device)->GetDevice();

    // 인덱스 버퍼 생성
    if (FAILED(device->CreateIndexBuffer(
        (cxHeight - 1) * (czHeight - 1) * 2 * sizeof(MyIndex),
        NULL, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &ib, NULL)))
    {
        return E_FAIL;
    }

    MyIndex* pi = NULL;

    // 인덱스 버퍼 락
    if (FAILED(ib->Lock(0, (cxHeight - 1) * (czHeight - 1) * 2 * sizeof(MyIndex),
        (void**)&pi, NULL)))
    {
        return E_FAIL;
    }

    MyIndex i;
    for (DWORD z = 0; z < czHeight - 1; ++z)
    {
        for (DWORD x = 0; x < cxHeight - 1; ++x)
        {
            // CW(시계) 방향으로 트라이앵글을 구성
            // -> 컬링이 CCW(반시계)로 설정 되어있기 때문에
            i._0 = z * cxHeight + x;
            i._1 = z * cxHeight + x + 1;
            i._2 = (z + 1) * cxHeight + x;
            *pi++ = i;

            i._0 = (z + 1) * cxHeight + x;
            i._1 = z * cxHeight + x + 1;
            i._2 = (z + 1) * cxHeight + x + 1;
            *pi++ = i;
        }
    }

    ib->Unlock();

    return S_OK;
}

void Terrain::DrawMesh()
{
    auto device = GET_SINGLE(Device)->GetDevice();

    // 월드 행렬 적용
    device->SetTransform(D3DTS_WORLD, &matRot);
    // 버텍스 버퍼 원본 소스 설정
    device->SetStreamSource(0, vb, 0, sizeof(TV));
    // FVF 설정
    device->SetFVF(D3DFVF_CUSTOMVERTEX);
    // 인덱스 버퍼 설정
    device->SetIndices(ib);

    device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
        cxHeight * czHeight, 0, (cxHeight - 1) * (czHeight - 1) * 2);
}

void Terrain::Animate()
{
    // static 지역 변수 선언 시, 맨 처음 한 번만 초기화되고 값이 유지됨
    static ULONGLONG t = 0;
    static bool flag = false;

    // 0 ~ 2pi 까지의 값이 담김
    ULONGLONG d = GetTickCount64() % (int)(D3DX_PI * 2 * 1000);

    // y축 회전 행렬
    D3DXMatrixRotationY(&matRot, /*d / 1000.f*/0);

    // 360도 회전을 한 번 할 때마다 플래그가 변경됨
    if (d < t) flag = !flag;

    //GET_SINGLE(Device)->GetDevice()->SetRenderState(D3DRS_FILLMODE,
    //    flag ? D3DFILL_WIREFRAME : D3DFILL_SOLID);

    t = d;
}
