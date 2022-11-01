

#include <GameSystem/Component/Renderer/boxRenderer.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Manager/sceneManager.h>

using namespace MySpace::Game;
using namespace MySpace::Graphics;

CBoxRenderer::CBoxRenderer(std::shared_ptr<CGameObject> ptr)
	:CMeshRenderer(ptr), m_vSize(50, 50, 50),m_pBBox(nullptr)
{
	SetBox(m_vSize);
}
CBoxRenderer::~CBoxRenderer()
{

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

	XMVECTOR vCenter = XMLoadFloat3(&GetCenter());
	XMMATRIX mWorld = XMLoadFloat4x4(&Transform()->GetWorldMatrix());

	vCenter = XMVector3TransformCoord(vCenter, mWorld);
	mWorld = XMMatrixTranslationFromVector(vCenter);
	XMFLOAT4X4 mW;
	XMStoreFloat4x4(&mW, mWorld);
	if (!m_pBBox)return false;

	m_pBBox->SetWorld(&mW);
	m_pBBox->Draw();
	return true;
}
HRESULT CBoxRenderer::SetBox(Vector3 vBBox)
{
	HRESULT hr = S_OK;

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
void CBoxRenderer::SetMaterial(CMeshMaterial mat)
{
	m_pBBox->SetMaterial(&mat);
}