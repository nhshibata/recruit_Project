
#include <GameSystem/Component/Renderer/sphereRenderer.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GraphicsSystem/DirectX/DXDevice.h>
#include <ImGui/imgui.h>

using namespace MySpace::Game;

CSphereRenderer::CSphereRenderer(std::shared_ptr<CGameObject> ptr)
	:CMeshRenderer(ptr), m_fRadius(50),m_pBSphere(nullptr)
{
	SetSphere(m_fRadius);
}
CSphereRenderer::~CSphereRenderer()
{
	m_pBSphere->Fin();
	m_pBSphere.reset();
}
void CSphereRenderer::Awake()
{	
	if (!m_pBSphere)SetSphere(m_fRadius);
}
void CSphereRenderer::Init()
{
	CMeshRenderer::Init();
}
void CSphereRenderer::Update()
{
	CMeshRenderer::Update();
	
}
bool CSphereRenderer::Draw()
{
	if (!CMeshRenderer::Draw())return false;
	m_pBSphere->SetMaterial(GetMaterial());

	XMVECTOR vCenter = XMLoadFloat3(&GetCenter());
	XMMATRIX mWorld = XMLoadFloat4x4(&Transform()->GetWorldMatrix());

	vCenter = XMVector3TransformCoord(vCenter, mWorld);
	mWorld = XMMatrixTranslationFromVector(vCenter);
	XMFLOAT4X4 mW;
	XMStoreFloat4x4(&mW, mWorld);
	if (!m_pBSphere)return false;

	m_pBSphere->SetWorld(&mW);
	this->SetInstancing(m_pBSphere.get());

	//m_pBSphere->Draw();
	return true;
}
HRESULT CSphereRenderer::SetSphere(float radius)
{
	HRESULT hr = S_OK;
	
	if(!m_pBSphere)
		m_pBSphere = std::make_shared<CSphere>();
	m_fRadius = radius;
	
	// TODO: •ªŠ„”
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

	if(ImGui::Button(u8"SphereRenderer"))
		m_fRadius = Transform()->GetScale().GetLargeValue();
	if (ImGui::DragFloat("Radius", &m_fRadius))
	{
		SetSphere(m_fRadius);
	}
	CMeshRenderer::ImGuiDebug();
}

#endif // 0