//=========================================================
// [sphereRenderer.cpp]
// �쐬: 2022/06/27
// �X�V: 2022/12/10 �C���X�^���V���O�`��Ή�
//---------------------------------------------------------
// 3D�`��
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Renderer/sphereRenderer.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GraphicsSystem/DirectX/DXDevice.h>

#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Game;

//=========================================================
// �R���X�g���N�^
//=========================================================
CSphereRenderer::CSphereRenderer(std::shared_ptr<CGameObject> ptr)
	:CMeshRenderer(ptr), m_fRadius(1),m_pBSphere(nullptr)
{
	SetSphere(m_fRadius);
}

//=========================================================
// �f�X�N�g���N�^
//=========================================================
CSphereRenderer::~CSphereRenderer()
{
	m_pBSphere->Fin();
	m_pBSphere.reset();
}

//=========================================================
// �ǂݍ��ݎ��Ăяo��
//=========================================================
void CSphereRenderer::OnLoad()
{	
	CRenderer::OnLoad();
	SetSphere(m_fRadius);
}

//=========================================================
// �������Ăяo��
//=========================================================
void CSphereRenderer::Awake()
{	
	if (!m_pBSphere)
	{
		SetSphere(m_fRadius);
		auto mW = m_pBSphere->GetWorld();
		Vector3 scale = Vector3(
			sqrtf(mW.m[0][0] + mW.m[1][0] + mW.m[2][0]),
			sqrtf(mW.m[0][1] + mW.m[1][1] + mW.m[2][1]),
			sqrtf(mW.m[0][2] + mW.m[1][2] + mW.m[2][2])
		);
		SetBSRadius(scale.GetLargeValue() * m_fRadius);
	}
}

//=========================================================
// ������
//=========================================================
void CSphereRenderer::Init()
{
	CMeshRenderer::Init();
}

//=========================================================
// �X�V
//=========================================================
void CSphereRenderer::Update()
{
	CMeshRenderer::Update();
}

//=========================================================
// �`��
//=========================================================
bool CSphereRenderer::Draw()
{
	if (!CMeshRenderer::Draw())
		return false;
	if (!m_pBSphere)
		return false;

	m_pBSphere->SetMaterial(GetMaterial());

	//--- �}�g���b�N�X�ݒ�
	XMVECTOR vCenter = XMLoadFloat3(&GetCenter());
	XMMATRIX mWorld = XMLoadFloat4x4(&Transform()->GetWorldMatrix());
	vCenter = XMVector3TransformCoord(vCenter, mWorld);
	mWorld = XMMatrixTranslationFromVector(vCenter);
	XMFLOAT4X4 mW;
	XMStoreFloat4x4(&mW, mWorld);

	m_pBSphere->SetWorld(&mW);

	//--- �C���X�^���V���O�`��˗�
	this->SetInstancing(m_pBSphere.get());

	//m_pBSphere->Draw();
	return true;
}

//=========================================================
// Mesh�\�z
// ���a�ݒ�
//=========================================================
HRESULT CSphereRenderer::SetSphere(float radius)
{
	HRESULT hr = S_OK;
	
	if (!m_pBSphere)
		m_pBSphere.reset();
	m_pBSphere = std::make_shared<CSphere>();

	m_fRadius = radius;
	
	// TODO: ������
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

	Debug::SetTextAndAligned("Renderer SphereRadius");

	if (ImGui::DragFloat("##Renderer SphereRadius", &m_fRadius))
	{
		SetSphere(m_fRadius);
	}

	ImGui::Separator();

	CMeshRenderer::ImGuiDebug();
}

#endif // 0