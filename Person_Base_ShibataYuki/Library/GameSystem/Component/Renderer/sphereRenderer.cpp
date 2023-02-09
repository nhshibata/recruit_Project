//=========================================================
// [sphereRenderer.cpp]
// 作成: 2022/06/27
// 更新: 2022/12/10 インスタンシング描画対応
//---------------------------------------------------------
// 3D描画
//=========================================================

//--- インクルード部
#include <GameSystem/Component/Renderer/sphereRenderer.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GraphicsSystem/DirectX/DXDevice.h>
#include <ImGui/imgui.h>

using namespace MySpace::Game;

//=========================================================
// コンストラクタ
//=========================================================
CSphereRenderer::CSphereRenderer(std::shared_ptr<CGameObject> ptr)
	:CMeshRenderer(ptr), m_fRadius(1),m_pBSphere(nullptr)
{
	SetSphere(m_fRadius);
}

//=========================================================
// デスクトラクタ
//=========================================================
CSphereRenderer::~CSphereRenderer()
{
	m_pBSphere->Fin();
	m_pBSphere.reset();
}

//=========================================================
// 読み込み時呼び出し
//=========================================================
void CSphereRenderer::OnLoad()
{	
	CRenderer::OnLoad();
	SetSphere(m_fRadius);
}

//=========================================================
// 生成時呼び出し
//=========================================================
void CSphereRenderer::Awake()
{	
	if (!m_pBSphere)
		SetSphere(m_fRadius);
}

//=========================================================
// 初期化
//=========================================================
void CSphereRenderer::Init()
{
	CMeshRenderer::Init();
}

//=========================================================
// 更新
//=========================================================
void CSphereRenderer::Update()
{
	CMeshRenderer::Update();
}

//=========================================================
// 描画
//=========================================================
bool CSphereRenderer::Draw()
{
	if (!CMeshRenderer::Draw())
		return false;
	if (!m_pBSphere)
		return false;

	m_pBSphere->SetMaterial(GetMaterial());

	//--- マトリックス設定
	XMVECTOR vCenter = XMLoadFloat3(&GetCenter());
	XMMATRIX mWorld = XMLoadFloat4x4(&Transform()->GetWorldMatrix());
	vCenter = XMVector3TransformCoord(vCenter, mWorld);
	mWorld = XMMatrixTranslationFromVector(vCenter);
	XMFLOAT4X4 mW;
	XMStoreFloat4x4(&mW, mWorld);

	m_pBSphere->SetWorld(&mW);

	//--- インスタンシング描画依頼
	this->SetInstancing(m_pBSphere.get());

	//m_pBSphere->Draw();
	return true;
}

//=========================================================
// Mesh構築
// 半径設定
//=========================================================
HRESULT CSphereRenderer::SetSphere(float radius)
{
	HRESULT hr = S_OK;
	
	if (!m_pBSphere)
		m_pBSphere.reset();
	m_pBSphere = std::make_shared<CSphere>();

	m_fRadius = radius;
	
	// TODO: 分割数
	hr = m_pBSphere->Init(16, 8, m_fRadius);
	if (FAILED(hr))
	{
		m_pBSphere->Fin();
		m_pBSphere.reset();
	}
	return hr;
}


#if BUILD_MODE

void CSphereRenderer::ImGuiDebug()
{
	//this->SetMaterial(*this->GetMaterial());

	//if(ImGui::Button(u8"SphereRenderer"))
	//m_fRadius = Transform()->GetScale().GetLargeValue();
	if (ImGui::DragFloat("SphereRadius renderer", &m_fRadius))
	{
		SetSphere(m_fRadius);
	}
	CMeshRenderer::ImGuiDebug();
}

#endif // 0