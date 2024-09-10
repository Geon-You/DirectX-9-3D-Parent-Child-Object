#include "KeyAnimObj.h"
#include "Device.h"

void KeyAnimObj::Initialize()
{
    InitAnimation();
    InitGeometry();
}

void KeyAnimObj::Update()
{
    Animate();
}

void KeyAnimObj::Render()
{
    D3DXMATRIXA16 matWorld;

    // 부모 객체의 월드 행렬 연산
    matWorld = matTransParent * matRotParent;
    // 부모 큐브 그리기
    DrawMesh(&matWorld);

    // 자식 객체 월드 행렬 연산
    matWorld = matTransChild * matRotChild * matWorld;
    // 크자이공부 -> 크기 자전 이동 공전 부모 순서로 연산
    // 자식 큐브 그리기
    DrawMesh(&matWorld);
}

void KeyAnimObj::Release()
{
    if (vb)
    {
        vb->Release();
        vb = nullptr;
    }

    if (ib)
    {
        ib->Release();
        ib = nullptr;
    }
}

HRESULT KeyAnimObj::InitVB()
{
    // 큐브를 렌더링하기 위해 8개의 정점 선언
    VA vertices[] =
    {
        { -1, 1, 1, 0xffff0000 },
        { 1, 1, 1, 0xff00ff00 },
        { 1, 1, -1, 0xff0000ff },
        { -1, 1, -1, 0xffffff00 },

        { -1, -1, 1, 0xff00ffff },
        { 1, -1, 1, 0xffff00ff },
        { 1, -1, -1, 0xff000000 },
        { -1, -1, -1, 0xffffffff }
    };

    auto device = GET_SINGLE(Device)->GetDevice();

    // 정점 버퍼 생성
    if (FAILED(device->CreateVertexBuffer(sizeof(VA) * 8,
        NULL, D3DFVF_CUSTOMVERTEX_ANIM, D3DPOOL_DEFAULT, &vb, NULL)))
        return E_FAIL;

    VOID* pVertices;

    // 정점 버퍼 락
    if (FAILED(vb->Lock(0, sizeof(vertices), (void**)&pVertices, NULL)))
        return E_FAIL;

    // 위쪽에 미리 정의해둔 버텍스 데이터들을 복사
    memcpy_s(pVertices, sizeof(vertices), vertices, sizeof(vertices));

    // 정점 버퍼 언락
    vb->Unlock();

    return S_OK;
}

HRESULT KeyAnimObj::InitIB()
{
    MYINDEXANIM indices[] =
    {
        {0, 1, 2}, {0, 2, 3}, // 윗면
        {4, 6, 5}, {4, 7, 6}, // 아랫면
        {0, 3, 7}, {0, 7, 4}, // 왼쪽
        {1, 5, 6}, {1, 6, 2}, // 오른쪽
        {3, 2, 6}, {3, 6, 7}, // 앞면
        {0, 4, 5}, {0, 5, 1}, // 뒷면
    };

    auto device = GET_SINGLE(Device)->GetDevice();

    // 인덱스 버퍼 생성
    if (FAILED(device->CreateIndexBuffer(sizeof(MYINDEXANIM) * 12, NULL,
        D3DFMT_INDEX16, D3DPOOL_DEFAULT, &ib, NULL)))
        return E_FAIL;

    // 인덱스 버퍼 락
    VOID* pIndices;
    if (FAILED(ib->Lock(0, sizeof(indices), (void**)&pIndices, NULL)))
        return E_FAIL;

    // 미리 정의해둔 인덱스 정보 복사
    memcpy_s(pIndices, sizeof(indices), indices, sizeof(indices));

    // 인덱스 버퍼 언락
    ib->Unlock();

    return S_OK;
}

HRESULT KeyAnimObj::InitGeometry()
{
    if (FAILED(InitVB())) return E_FAIL;
    if (FAILED(InitIB())) return E_FAIL;

    return S_OK;
}

void KeyAnimObj::InitAnimation()
{
    // 시작 위치 값
    aniPos[0] = D3DXVECTOR3(0, 0, 0);
    // 종료 위치 값
    aniPos[1] = D3DXVECTOR3(5, 5, 5);
    // 시작 회전 값
    D3DXVECTOR3 ypr = D3DXVECTOR3(D3DX_PI * 90.f / 180.f, 0, 0);
    D3DXQuaternionRotationYawPitchRoll(&aniRot[0], ypr.x, ypr.y, ypr.z);
    // 종료 회전 값
    ypr = D3DXVECTOR3(0, D3DX_PI * 90.f / 180.f, 0);
    D3DXQuaternionRotationYawPitchRoll(&aniRot[1], ypr.x, ypr.y, ypr.z);
}

void KeyAnimObj::Animate()
{
    // 애니메이션에 사용할 시간 값
    static float t = 0;

    // 시간값을 0 ~ 1 사이의 값으로 사용 (정규화해서 사용할 예정)
    // 0 : 애니메이션 시작 시간, 1 : 애니메이션 종료 시간
    if (t > 1.f) t = 0;

    // 보간된 위치 값을 담을 변수
    D3DXVECTOR3 pos = D3DXVECTOR3(Linear(aniPos[0].x, aniPos[1].x, t),
        Linear(aniPos[0].y, aniPos[1].y, t),
        Linear(aniPos[0].z, aniPos[1].z, t));

    // 벡터 보간 기능
    // D3DXVec3Lerp();

    // 보간된 위치를 부모 객체의 위치 행렬에 적용
    D3DXMatrixTranslation(&matTransParent, pos.x, pos.y, pos.z);

    // 보간된 회전 값을 담을 변수
    D3DXQUATERNION quat;

    // 회전 값을 구면 선형 보간
    D3DXQuaternionSlerp(&quat, &aniRot[0], &aniRot[1], t);
    // 보간된 회전 값을 갖는 쿼터니언을 부모의 회전 행렬에 적용
    D3DXMatrixRotationQuaternion(&matRotParent, &quat);

    // 부모가 존재하므로, 부모를 기준으로 3, 3, 3만큼 떨어진 위치
    D3DXMatrixTranslation(&matTransChild, 3.f, 3.f, 3.f);
    // 로컬 회전은 존재하지 않게 설정
    D3DXMatrixIdentity(&matRotChild);

    t += 0.003f;
}

void KeyAnimObj::DrawMesh(D3DXMATRIXA16* mat)
{
    auto device = GET_SINGLE(Device)->GetDevice();

    // 광원 없으므로 조명 끔
    device->SetRenderState(D3DRS_LIGHTING, FALSE);

    device->SetTransform(D3DTS_WORLD, mat);
    device->SetStreamSource(0, vb, 0, sizeof(VA));
    device->SetFVF(D3DFVF_CUSTOMVERTEX_ANIM);

    device->SetIndices(ib);
    device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
}

float KeyAnimObj::Linear(float a, float b, float t)
{
    // a의 성분은 1에서 0으로 점점 감소
    // b의 성분은 0에서 1로 점점 증가
    return a * (1.f - t) + b * t;
}
