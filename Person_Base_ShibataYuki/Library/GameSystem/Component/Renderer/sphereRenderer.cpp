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
#include <ImGui/imgui.h>

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
		SetSphere(m_fRadius);
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