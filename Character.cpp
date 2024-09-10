#include "Character.h"
#include "Device.h"

void Character::Initialize()
{
	pos = D3DXVECTOR3(0, 100.f, 0);

	// 재질을 임시로 보관할 버퍼 선언
	LPD3DXBUFFER matBuffer;

	auto device = GET_SINGLE(Device)->GetDevice();

	// x파일을 불러옵니다.
	if (FAILED(D3DXLoadMeshFromX(L"Ezreal/Iz.x", D3DXMESH_SYSTEMMEM,
		device, NULL, &matBuffer, NULL, &matNum, &mesh)))
	{
		return;
	}

	// 재질 정보와 텍스쳐 정보를 분리해서 가져옵니다.
	D3DXMATERIAL* mat = (D3DXMATERIAL*)matBuffer->GetBufferPointer();

	// 재질 개수만큼 재질 배열을 생성
	meshMat = new D3DMATERIAL9[matNum];
	// 재질 개수만큼 텍스쳐 배열을 생성
	meshTex = new LPDIRECT3DTEXTURE9[matNum];

	// 재질 개수만큼 반복하며, 실제 재질 정보 가져오기와 텍스쳐 로드를 실행
	for (int i = 0; i < matNum; ++i)
	{
		// 재질 정보 가져오기
		meshMat[i] = mat[i].MatD3D;

		// 주변 광원 데이터를 재질에 있는 반사광으로 설정
		meshMat[i].Ambient = meshMat[i].Diffuse;

		meshTex[i] = NULL;

		// 재질에 사용되는 메인텍스쳐 이름을 통해, 텍스쳐를 로드한다.
		if (mat[i].pTextureFilename != NULL &&
			strlen(mat[i].pTextureFilename) > 0)
		{
			// 이 쪽으로 들어왔다는 것은, 사용하는 메인텍스쳐가 존재한다는 것
			char filePath[256];
			sprintf_s(filePath, "Ezreal/%s", mat[i].pTextureFilename);

			// 텍스쳐 로드
			if (FAILED(D3DXCreateTextureFromFileA(device, filePath, &meshTex[i])))
			{
				return;
			}
		}
	}

	// 임시로 생성한 재질 버터 해제
	matBuffer->Release();
}

void Character::Update()
{
}

void Character::Render()
{
	auto device = GET_SINGLE(Device)->GetDevice();
	D3DXMATRIX matWorld, matTrans, matScale;

	// 재질 개수만큼 반복해서 그림
	for (int i = 0; i < matNum; ++i)
	{
		// 재질과 텍스쳐를 설정
		device->SetMaterial(&meshMat[i]);
		device->SetTexture(0, meshTex[i]);

		// 행렬 적용
		D3DXMatrixTranslation(&matTrans, pos.x, pos.y, pos.z);
		D3DXMatrixScaling(&matScale, .5f, .5f, .5f);

		matWorld = matScale * matTrans;
		device->SetTransform(D3DTS_WORLD, &matWorld);

		// 메시 그리기
		mesh->DrawSubset(i);
	}
}

void Character::Release()
{
	if (mesh)
	{
		mesh->Release();
		mesh = nullptr;
	}

	if (meshMat)
	{
		delete[] meshMat;
		meshMat = nullptr;
	}

	if (meshTex)
	{
		delete[] meshTex;
		meshTex = nullptr;
	}
}
