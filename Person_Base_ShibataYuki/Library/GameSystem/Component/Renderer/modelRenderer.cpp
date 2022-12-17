//=========================================================
// [modelRenderer.h] 
// �쐬: 2022/06/27
// �X�V: 2022/07/04 ����
// �X�V: 2022/12/08 �C���X�^���V���O�ɑΉ�
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Renderer/modelRenderer.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/drawSystem.h>

#include <GraphicsSystem/DirectX/DXDevice.h>
#include <DebugSystem/imGuiPackage.h>
#include <CoreSystem/File/filePath.h>

using namespace MySpace::System;
using namespace MySpace::Game;
using namespace MySpace::Graphics;

CModelRenderer::CModelRenderer(std::shared_ptr<CGameObject> owner)
	:CMeshRenderer(owner), m_modelName(CHARACTER_PATH(mukade_head.obj)),
	m_pIndex(nullptr), m_pVertex(nullptr), m_nVertex(0), m_nIndex(0),
	m_bInstancing(false)
{
	
}
CModelRenderer::~CModelRenderer()
{
	if(m_pIndex && m_pVertex)
		FinVertexArray();
	if (m_pModel)
	{
		// ���L���̕���
		m_pModel.reset();
	}
}
void CModelRenderer::SetModel(std::string name)
{
	// �|�C���^���󂯎��
	if (m_pModel = CModelManager::Get()->GetModel(name); m_pModel)
	{
		m_modelName = name;

		// ���f�����a�̐ݒ�
		auto size = Transform()->GetScale();
		float scale = size.x;
		if (scale < size.y || scale < size.z)
		{
			scale = size.y;
			if (scale < size.z)
				scale = size.z;
		}
		SetBSRadius(m_pModel->GetRadius() * scale);

		// ���g�ƊǗ��׽�ȊO�ɏ��L�҂�������
		if (int num = CModelManager::Get()->GetModelCnt(name); num > 2)
		{
			m_bInstancing = true;
		}
	}

	// ���_�z��A�C���f�b�N�X�z����擾���Ă���
	InitVertexArray();
}
void CModelRenderer::Awake()
{
	GetOwner()->SetLayer(CLayer::E_Layer::MODEL);
	SetModel(m_modelName);
}
void CModelRenderer::Init()
{
	//SetModel(m_modelName);
	CMeshRenderer::Init();
}
void CModelRenderer::Update()
{
}
bool CModelRenderer::Draw()
{
	if (!CMeshRenderer::Draw())return false;
	if (!m_pModel)return false;

	XMFLOAT4X4 mtx = Transform()->GetWorldMatrix();
	//--- �C���X�^���V���O�`��
	// ���g�ƊǗ��ȊO���������Ă��邩�m�F
	if (m_bInstancing || CModelManager::Get()->GetModelCnt(m_modelName) > 2)
	{
		// �V�X�e�����Ɉ˗����o���A�܂Ƃ߂ĕ`�悵�Ă��炤
		SceneManager::CSceneManager::Get()->GetDrawSystem()->SetInstanchingModel(m_modelName, mtx);
		return true;
	}

	//--- �s�����`��
	CLight* pLight = CLight::Get();
	if (!pLight)return false;
	pLight->SetDisable(GetLightEnable());	// ���C�e�B���O����
	
	m_pModel->Draw(CDXDevice::Get()->GetDeviceContext(), mtx, EByOpacity::eOpacityOnly);

	pLight->SetEnable();	// ���C�e�B���O�L��

	//--- �����������`��
	CDXDevice::Get()->SetZBuffer(false);
	CDXDevice::Get()->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));

	m_pModel->Draw(CDXDevice::Get()->GetDeviceContext(), mtx, EByOpacity::eTransparentOnly);

	CDXDevice::Get()->SetZBuffer(true);			// ���u�����f�B���O����
	CDXDevice::Get()->SetBlendState(static_cast<int>(EBlendState::BS_NONE));		// �����L��

	return true;
}
bool CModelRenderer::Draw(int no)
{
	XMFLOAT4X4 mtx = Transform()->GetWorldMatrix();
	
	if (!m_pModel)return false;

	//--- �s�����`��
	CLight* pLight = CLight::Get();
	if (!pLight)return false;
	pLight->SetDisable(GetLightEnable());	// ���C�e�B���O����

	m_pModel->Draw(CDXDevice::Get()->GetDeviceContext(), mtx, EByOpacity::eOpacityOnly);

	pLight->SetEnable();	// ���C�e�B���O�L��

	//--- �����������`��
	CDXDevice::Get()->SetZBuffer(false);
	CDXDevice::Get()->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));

	m_pModel->Draw(CDXDevice::Get()->GetDeviceContext(), mtx, EByOpacity::eTransparentOnly);

	CDXDevice::Get()->SetZBuffer(true);			// ���u�����f�B���O����
	CDXDevice::Get()->SetBlendState(static_cast<int>(EBlendState::BS_NONE));		// �����L��

	return true;
}
// ���_/�C���f�b�N�X�z����
void CModelRenderer::FinVertexArray()
{
	SAFE_DELETE_ARRAY(m_pIndex);
	SAFE_DELETE_ARRAY(m_pVertex);
}
void CModelRenderer::InitVertexArray()
{
	FinVertexArray();
	CAssimpModel* pModel = m_pModel.get();
	if (!pModel) return;
	pModel->GetVertexCount(&m_nVertex, &m_nIndex);
	m_pVertex = new TAssimpVertex[m_nVertex];
	m_pIndex = new UINT[m_nIndex];
	pModel->GetVertex(m_pVertex, m_pIndex);
}
// ���C�Ƃ̓����蔻��
bool CModelRenderer::CollisionRay(XMFLOAT3 vP0, XMFLOAT3 vW, XMFLOAT3* pX, XMFLOAT3* pN)
{
	// �S�Ă̎O�p�`�ɂ��ČJ��Ԃ�
	for (UINT i = 0; i < m_nIndex; )
	{
		// �O�p�`��3���_
		XMFLOAT3& vV0 = m_pVertex[m_pIndex[i++]].vPos;
		XMFLOAT3& vV1 = m_pVertex[m_pIndex[i++]].vPos;
		XMFLOAT3& vV2 = m_pVertex[m_pIndex[i++]].vPos;
		// 2�ӂ��@���x�N�g�������߂�
		XMVECTOR v0v1 = XMVectorSet(vV1.x - vV0.x, vV1.y - vV0.y, vV1.z - vV0.z, 0.0f);
		XMVECTOR v1v2 = XMVectorSet(vV2.x - vV1.x, vV2.y - vV1.y, vV2.z - vV1.z, 0.0f);
		XMVECTOR n = XMVector3Normalize(XMVector3Cross(v0v1, v1v2));
		// �@���x�N�g���ƃ��C�̕����x�N�g���Ƃ̓���(���̕���)�����߂�
		float base;
		XMStoreFloat(&base, XMVector3Dot(n, XMLoadFloat3(&vW)));
		// ���ꂪ0�Ȃ�ʂƕ��s�̂��ߎ��̎O�p�`��
		if (base == 0.0f)
		{
			continue;
		}
		// �}��ϐ�t�����߂�
		float t;
		XMStoreFloat(&t, XMVector3Dot(n, XMVectorSet(vV0.x - vP0.x, vV0.y - vP0.y, vV0.z - vP0.z, 0.0f)));
		t /= base;
		// t�����Ȃ��_�̓��C�̌��Ȃ̂Ŏ��̎O�p�`��
		if (t < 0.0f)
		{
			continue;
		}
		// ��_X�����߂�
		XMFLOAT3 vX;
		vX.x = vP0.x + t * vW.x;
		vX.y = vP0.y + t * vW.y;
		vX.z = vP0.z + t * vW.z;
		// ��_���O�p�`�̓��������ׂ�
		float dot;
		XMStoreFloat(&dot, XMVector3Dot(n,
										XMVector3Cross(v0v1, XMVectorSet(vX.x - vV0.x, vX.y - vV0.y, vX.z - vV0.z, 0.0f))));
		if (dot < 0.0f)
		{
			continue;
		}
		XMStoreFloat(&dot, XMVector3Dot(n,
										XMVector3Cross(v1v2, XMVectorSet(vX.x - vV1.x, vX.y - vV1.y, vX.z - vV1.z, 0.0f))));
		if (dot < 0.0f)
		{
			continue;
		}
		XMVECTOR v2v0 = XMVectorSet(vV0.x - vV2.x, vV0.y - vV2.y, vV0.z - vV2.z, 0.0f);
		XMStoreFloat(&dot, XMVector3Dot(n,
										XMVector3Cross(v2v0, XMVectorSet(vX.x - vV2.x, vX.y - vV2.y, vX.z - vV2.z, 0.0f))));
		if (dot < 0.0f)
		{
			continue;
		}
		// �_�������������̂œ������Ă���
		if (pX)
		{
			*pX = vX;
		}
		if (pN)
		{
			XMStoreFloat3(pN, n);
		}
		return true;
	}
	return false;	// �������Ă��Ȃ�
}

// �����Ƃ̓����蔻��
bool CModelRenderer::CollisionLineSegment(XMFLOAT3 vP0, XMFLOAT3 vP1, XMFLOAT3* pX, XMFLOAT3* pN)
{
	// �S�Ă̎O�p�`�ɂ��ČJ��Ԃ�
	for (UINT i = 0; i < m_nIndex; )
	{
		// �O�p�`��3���_
		XMFLOAT3& vV0 = m_pVertex[m_pIndex[i++]].vPos;
		XMFLOAT3& vV1 = m_pVertex[m_pIndex[i++]].vPos;
		XMFLOAT3& vV2 = m_pVertex[m_pIndex[i++]].vPos;
		// 2�ӂ��@���x�N�g�������߂�
		XMVECTOR v0v1 = XMVectorSet(vV1.x - vV0.x, vV1.y - vV0.y, vV1.z - vV0.z, 0.0f);
		XMVECTOR v1v2 = XMVectorSet(vV2.x - vV1.x, vV2.y - vV1.y, vV2.z - vV1.z, 0.0f);
		XMVECTOR n = XMVector3Normalize(XMVector3Cross(v0v1, v1v2));
		// �@���x�N�g���Ɛ����̕����x�N�g���Ƃ̓���(���̕���)�����߂�
		XMFLOAT3 vW;
		vW.x = vP1.x - vP0.x;
		vW.y = vP1.y - vP0.y;
		vW.z = vP1.z - vP0.z;
		float base;
		XMStoreFloat(&base, XMVector3Dot(n, XMLoadFloat3(&vW)));
		// ���ꂪ0�Ȃ�ʂƕ��s�̂��ߎ��̎O�p�`��
		if (base == 0.0f)
		{
			continue;
		}
		// �}��ϐ�t�����߂�
		float t;
		XMStoreFloat(&t, XMVector3Dot(n, XMVectorSet(vV0.x - vP0.x, vV0.y - vP0.y, vV0.z - vP0.z, 0.0f)));
		t /= base;
		// t�����Ȃ��_�͐����̌��Ȃ̂Ŏ��̎O�p�`��
		if (t < 0.0f)
		{
			continue;
		}
		// t��1���傫���ꍇ�͐����̐�Ȃ̂Ŏ��̎O�p�`��
		if (t > 1.0f)
		{
			continue;
		}
		// ��_X�����߂�
		XMFLOAT3 vX;
		vX.x = vP0.x + t * vW.x;
		vX.y = vP0.y + t * vW.y;
		vX.z = vP0.z + t * vW.z;
		// ��_���O�p�`�̓��������ׂ�
		float dot;
		XMStoreFloat(&dot, XMVector3Dot(n,
										XMVector3Cross(v0v1, XMVectorSet(vX.x - vV0.x, vX.y - vV0.y, vX.z - vV0.z, 0.0f))));
		if (dot < 0.0f)
		{
			continue;
		}
		XMStoreFloat(&dot, XMVector3Dot(n,
										XMVector3Cross(v1v2, XMVectorSet(vX.x - vV1.x, vX.y - vV1.y, vX.z - vV1.z, 0.0f))));
		if (dot < 0.0f)
		{
			continue;
		}
		XMVECTOR v2v0 = XMVectorSet(vV0.x - vV2.x, vV0.y - vV2.y, vV0.z - vV2.z, 0.0f);
		XMStoreFloat(&dot, XMVector3Dot(n,
										XMVector3Cross(v2v0, XMVectorSet(vX.x - vV2.x, vX.y - vV2.y, vX.z - vV2.z, 0.0f))));
		if (dot < 0.0f)
		{
			continue;
		}
		// �_�������������̂œ������Ă���
		if (pX)
		{
			*pX = vX;
		}
		if (pN)
		{
			XMStoreFloat3(pN, n);
		}
		return true;
	}
	return false;	// �������Ă��Ȃ�
}

#ifdef BUILD_MODE

void CModelRenderer::ImGuiDebug()
{
	using namespace MySpace::Debug;

	CMeshRenderer::ImGuiDebug();

	if (m_aXModelList.empty() && m_aObjModelList.empty() && m_aFbxModelList.empty() || ImGui::Button("���f�� reload"))
	{
		MySpace::System::CFilePath file;
		m_aXModelList = file.GetAllFileName(MODEL_PATH, ".x");
		m_aObjModelList = file.GetAllFileName(MODEL_PATH, ".obj");
		m_aFbxModelList = file.GetAllFileName(MODEL_PATH, ".fbx");
	}

	// ���O����
	m_modelName = InputString(m_modelName, u8"�ݒ胂�f��");

	// ���f��
	if (auto name = DispMenuBar(m_aXModelList, "xFile"); !name.empty())
	{
		m_modelName = name;

		// �|�C���^���󂯎��
		m_pModel = CModelManager::Get()->GetModel(name);
	}
	
	if (auto name = DispMenuBar(m_aObjModelList, "objFile"); !name.empty())
	{
		m_modelName = name;

		// �|�C���^���󂯎��
		m_pModel = CModelManager::Get()->GetModel(name);
	}
	
	if (auto name = DispMenuBar(m_aFbxModelList, "fbxFile"); !name.empty())
	{
		m_modelName = name;

		// �|�C���^���󂯎��
		m_pModel = CModelManager::Get()->GetModel(name);
		SetModel(name);
	}
}

#endif // BUILD_MODE