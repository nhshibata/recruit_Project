//=========================================================
// [boxRenderer.cpp] 
// 作成: 2022/06/27
// 更新: 2022/12/10 インスタンシング描画対応
//---------------------------------------------------------
// 3D描画
//=========================================================

//--- インクルード部
#include <GameSystem/Component/Renderer/boxRenderer.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GraphicsSystem/DirectX/DXDevice.h>

#include <ImGui/imgui.h>

using namespace MySpace::Game;
using namespace MySpace::Graphics;

//==========================================================
// 引き数付きコンストラクタ
//==========================================================
CBoxRenderer::CBoxRenderer(std::shared_ptr<CGameObject> ptr)
	:CMeshRenderer(ptr), m_vSize(1, 1, 1), m_pBBox(nullptr)
{
	SetBox(m_vSize);
}

//==========================================================
// デストラクタ
//==========================================================
CBoxRenderer::~CBoxRenderer()
{
	m_pBBox->Fin();
	m_pBBox.reset();
}

//==========================================================
// 生成時呼び出し
//==========================================================
void CBoxRenderer::Awake()
{
}

//==========================================================
// 初期化
//==========================================================
void CBoxRenderer::Init()
{
	CMeshRenderer::Init();
}

//==========================================================
// 更新
//==========================================================
void CBoxRenderer::Update()
{
	CMeshRenderer::Update();
}

//==========================================================
// 描画
//==========================================================
bool CBoxRenderer::Draw()
{
	if (!CMeshRenderer::Draw())return false;

	m_pBBox->SetMaterial(GetMaterial());

#pragma region AABB
	//XMVECTOR vCenter = XMLoadFloat3(&GetCenter());
	//XMMATRIX mWorld = XMLoadFloat4x4(&Transform()->GetWorldMatrix());

	//vCenter = XMVector3TransformCoord(vCenter, mWorld);
	//mWorld = XMMatrixTranslationFromVector(vCenter);
	//XMFLOAT4X4 mW;
	//XMStoreFloat4x4(&mW, mWorld);
	//if (!m_pBBox)return false;

	//m_pBBox->SetWorld(&mW);
#pragma endregion

#pragma region OBB
	//CDXDevice* dx = &CDXDevice::Get();
	// 平行移動マトリックス生成
	Vector3 center = GetCenter();
	XMMATRIX mMove = XMMatrixTranslation(center.x, center.y, center.z);
	// モデル
	XMMATRIX mWorld = XMLoadFloat4x4(&Transform()->GetWorldMatrix());
	mWorld = XMMatrixMultiply(mMove, mWorld);
	// 境界ボックス表示
	XMFLOAT4X4 mW;
	XMStoreFloat4x4(&mW, mWorld);
	m_pBBox->SetWorld(&mW);
#pragma endregion

	//--- インスタンシング描画依頼
	this->SetInstancing(m_pBBox.get());
	//m_pBBox->Draw();

	return true;
}

//==========================================================
// 箱サイズ設定
//==========================================================
HRESULT CBoxRenderer::SetBox(Vector3 vBBox)
{
	HRESULT hr = S_OK;

	if(!m_pBBox)
		m_pBBox = std::make_shared<CBox>();
	m_vSize = vBBox;
	hr = m_pBBox->Init(vBBox);
	if (FAILED(hr))
	{
		m_pBBox->Fin();
		m_pBBox.reset();
	}
	return hr;
}


#if BUILD_MODE

void CBoxRenderer::ImGuiDebug()
{
	if (ImGui::Button(u8"SphereRenderer"))
		m_vSize = Transform()->GetScale();
	if (ImGui::DragFloat3("Box", (float*)&m_vSize))
	{
		SetBox(m_vSize);
	}
	CMeshRenderer::ImGuiDebug();

}

#endif // 0