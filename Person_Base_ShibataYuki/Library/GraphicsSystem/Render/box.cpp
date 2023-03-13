//==========================================================
// [Box.cpp]
// 直方体 
//==========================================================

//--- インクルード部
#include <GraphicsSystem/Render/box.h>

using namespace MySpace::Graphics;

//==========================================================
// コンストラクタ
//==========================================================
CBox::CBox() : CMesh()
{
}

//==========================================================
// デストラクタ
//==========================================================
CBox::~CBox()
{
}

//==========================================================
// 初期化
//==========================================================
HRESULT CBox::Init(XMFLOAT3 vBBox)
{
	HRESULT hr = S_OK;
	XMFLOAT3 vSize;
	vSize.x = vBBox.x * 2.0f;
	vSize.y = vBBox.y * 2.0f;
	vSize.z = vBBox.z * 2.0f;

	const static float vtx[] = {
		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
	};
	const static float nor[] = {
		 0.0f,  0.0f, -1.0f,
		 1.0f,  0.0f,  0.0f,
		 0.0f,  0.0f,  1.0f,
		-1.0f,  0.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,
	};
	const static int face[] = {
		4, 0, 5, 1, 5, 0,
		5, 1, 7, 3, 7, 1,
		7, 3, 6, 2, 6, 3,
		6, 2, 4, 0, 4, 2,
		0, 2, 1, 3, 1, 2,
		6, 4, 7, 5, 7, 4,
	};
	const static float uv[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
	};

	const int vtxNum = _countof(vtx);

	VERTEX_3D* pVertexWk = new VERTEX_3D[vtxNum];
	int* pIndexWk = new int[36];
	VERTEX_3D* pVtx = pVertexWk;
	int nIdx = 0;
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 4; ++j) {
			pVtx->vtx.x = vtx[face[i * 6 + j] * 3 + 0] * vBBox.x;
			pVtx->vtx.y = vtx[face[i * 6 + j] * 3 + 1] * vBBox.y;
			pVtx->vtx.z = vtx[face[i * 6 + j] * 3 + 2] * vBBox.z;
			pVtx->nor.x = nor[i * 3 + 0];
			pVtx->nor.y = nor[i * 3 + 1];
			pVtx->nor.z = nor[i * 3 + 2];
			pVtx->tex.x = uv[j * 2 + 0];
			pVtx->tex.y = uv[j * 2 + 1];
			pVtx->diffuse = XMFLOAT4(1, 1, 1, 1);
			++pVtx;
			pIndexWk[i * 6 + j] = nIdx;
			++nIdx;
		}
		pIndexWk[i * 6 + 4] = pIndexWk[i * 6 + 2];
		pIndexWk[i * 6 + 5] = pIndexWk[i * 6 + 1];
	}

	hr = CMesh::Init(pVertexWk, vtxNum, pIndexWk, 36);

	delete[] pIndexWk;
	delete[] pVertexWk;
	return hr;
}
