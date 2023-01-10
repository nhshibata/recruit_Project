//=========================================================
// [boxRenderer.cpp] 
// �쐬: 2022/06/27
// �X�V: 2022/12/10 �C���X�^���V���O�`��Ή�
//---------------------------------------------------------
// 3D�`��
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Renderer/boxRenderer.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GraphicsSystem/DirectX/DXDevice.h>

#include <ImGui/imgui.h>

using namespace MySpace::Game;
using namespace MySpace::Graphics;

//==========================================================
// �������t���R���X�g���N�^
//==========================================================
CBoxRenderer::CBoxRenderer(std::shared_ptr<CGameObject> ptr)
	:CMeshRenderer(ptr), m_vSize(1, 1, 1), m_pBBox(nullptr)
{
	SetBox(m_vSize);
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CBoxRenderer::~CBoxRenderer()
{
	m_pBBox->Fin();
	m_pBBox.reset();
}

//==========================================================
// �������Ăяo��
//==========================================================
void CBoxRenderer::Awake()
{
}

//==========================================================
// ������
//==========================================================
void CBoxRenderer::Init()
{
	CMeshRenderer::Init();
}

//==========================================================
// �X�V
//==========================================================
void CBoxRenderer::Update()
{
	CMeshRenderer::Update();
}

//==========================================================
// �`��
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
	// ���s�ړ��}�g���b�N�X����
	Vector3 center = GetCenter();
	XMMATRIX mMove = XMMatrixTranslation(center.x, center.y, center.z);
	// ���f��
	XMMATRIX mWorld = XMLoadFloat4x4(&Transform()->GetWorldMatrix());
	mWorld = XMMatrixMultiply(mMove, mWorld);
	// ���E�{�b�N�X�\��
	XMFLOAT4X4 mW;
	XMStoreFloat4x4(&mW, mWorld);
	m_pBBox->SetWorld(&mW);
#pragma endregion

	//--- �C���X�^���V���O�`��˗�
	this->SetInstancing(m_pBBox.get());
	//m_pBBox->Draw();

	return true;
}

//==========================================================
// ���T�C�Y�ݒ�
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