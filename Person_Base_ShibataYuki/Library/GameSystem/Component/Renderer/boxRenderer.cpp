

#include <GameSystem/Component/Renderer/boxRenderer.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Manager/sceneManager.h>
#include <ImGui/imgui.h>
#include <GraphicsSystem/DirectX/DXDevice.h>

using namespace MySpace::Game;
using namespace MySpace::Graphics;

CBoxRenderer::CBoxRenderer(std::shared_ptr<CGameObject> ptr)
	:CMeshRenderer(ptr), m_vSize(50, 50, 50),m_pBBox(nullptr)
{
	SetBox(m_vSize);
}
CBoxRenderer::~CBoxRenderer()
{
	m_pBBox->Fin();
	m_pBBox.reset();
}
void CBoxRenderer::Awake()
{
}
void CBoxRenderer::Init()
{
	CMeshRenderer::Init();
}
void CBoxRenderer::Update()
{
	CMeshRenderer::Update();
}
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

	this->SetInstancing(m_pBBox.get());
	//m_pBBox->Draw();

	return true;
}
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