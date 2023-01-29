//=========================================================
// [modelRenderer.h] 
// �쐬: 2022/06/27
// �X�V: 2022/07/04 ����
// �X�V: 2022/12/08 �C���X�^���V���O�ɑΉ�
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h��
#include <Application/Application.h>
#include <GameSystem/Component/Renderer/modelRenderer.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/drawSystem.h>

#include <GraphicsSystem/DirectX/DXDevice.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <DebugSystem/imGuiPackage.h>
#include <CoreSystem/File/filePath.h>

using namespace MySpace::System;
using namespace MySpace::Game;
using namespace MySpace::Graphics;

//==========================================================
// �R���X�g���N�^
//==========================================================
CModelRenderer::CModelRenderer()
	:m_modelName(std::string()), m_nVertex(0), m_pVertex(nullptr), m_nIndex(0), m_pIndex(nullptr)
{	
}

//==========================================================
// �������R���X�g���N�^
//==========================================================
CModelRenderer::CModelRenderer(std::shared_ptr<CGameObject> owner)
	:CMeshRenderer(owner), m_modelName(CHARACTER_PATH(mukade_head.obj)),
	m_pIndex(nullptr), m_pVertex(nullptr), m_nVertex(0), m_nIndex(0)
{	
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CModelRenderer::~CModelRenderer()
{
	if(m_pIndex && m_pVertex)
		FinVertexArray();
	// ���L���̕���
	if (m_pModel)
		m_pModel.reset();
}

//==========================================================
// ���f���ݒ�
//==========================================================
bool CModelRenderer::SetModel(std::string name)
{
#if _DEBUG
	auto assets = Application::Get()->GetSystem<CAssetsManager>();
	if (!assets)
		return false;
	auto modelMgr = assets->GetModelManager();
#else
	auto modelMgr = Application::Get()->GetSystem<CAssetsManager>()->GetModelManager();
#endif // _DEBUG

	// ���f���擾,�|�C���^���󂯎��
	if (auto pModel = modelMgr->GetModel(name); pModel)
	{
		m_pModel = pModel;
		modelMgr->FinishUse(name);
		m_modelName = name;

		// ���f�����a�̐ݒ�
		/*auto size = Transform()->GetScale();
		float scale = size.x;
		if (scale < size.y || scale < size.z)
		{
			scale = size.y;
			if (scale < size.z)
				scale = size.z;
		}*/
		//SetBSRadius(m_pModel->GetRadius() * scale);
		SetBSRadius(m_pModel->GetRadius());
		
		// ���_�z��A�C���f�b�N�X�z����擾���Ă���
		InitVertexArray();
		
		return true;
	}
	return false;
}

//==========================================================
// �ǂݍ��ݎ��Ăяo��
//==========================================================
void CModelRenderer::OnLoad()
{
	CRenderer::OnLoad();
	SetModel(m_modelName);
}

//==========================================================
// �������Ăяo��
//==========================================================
void CModelRenderer::Awake()
{
	SetModel(m_modelName);
}

//==========================================================
// ������
//==========================================================
void CModelRenderer::Init()
{
	//SetModel(m_modelName);
	CMeshRenderer::Init();
}

//==========================================================
// �X�V
//==========================================================
void CModelRenderer::Update()
{
}

//==========================================================
// �`��
// �C���X�^���V���O�Ή�
// DrawSystem������Ăяo�����
//==========================================================
bool CModelRenderer::Draw()
{
	if (!CMeshRenderer::Draw())return false;
	if (!m_pModel)return false;

	auto pModelMgr = Application::Get()->GetSystem<CAssetsManager>()->GetModelManager();
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	XMFLOAT4X4 mtx = Transform()->GetWorldMatrix();

	//--- �C���X�^���V���O�`��

#if 1
	// �V�X�e�����Ɉ˗����o���A�܂Ƃ߂ĕ`�悵�Ă��炤
	SceneManager::CSceneManager::Get()->GetDrawSystem()->SetInstanchingModel(m_modelName, mtx);
	return true;

#else
	//--- �C���X�^���V���O�`��
	// ���g�ƊǗ��N���X�ȊO���������Ă��邩�m�F
	if (pModelMgr->GetModelCnt(m_modelName) > 2)
	{
		// �V�X�e�����Ɉ˗����o���A�܂Ƃ߂ĕ`�悵�Ă��炤
		SceneManager::CSceneManager::Get()->GetDrawSystem()->SetInstanchingModel(m_modelName, mtx);
		return true;
	}

	//--- �s�����`��
	CLight* pLight = CLight::GetMain();
	if (!pLight)return false;

	pLight->SetEnable(GetLightEnable());	// ���C�e�B���O�ύX
	
	m_pModel->Draw(pDX->GetDeviceContext(), mtx, EByOpacity::eOpacityOnly);

	pLight->SetEnable();	// ���C�e�B���O�L��

	return true;
#endif // 1

}

//==========================================================
// �`��
// �C���X�^���V���O��Ή�
//==========================================================
bool CModelRenderer::DrawAlpha()
{
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	XMFLOAT4X4 mtx = Transform()->GetWorldMatrix();

	//--- �����������`��
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));
	pDX->SetZWrite(false);

	m_pModel->Draw(pDX->GetDeviceContext(), mtx, EByOpacity::eTransparentOnly);

	pDX->SetZWrite(true);
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_NONE)); // ���u�����f�B���O����
	
	return true;
}

//==========================================================
// �`��
// �C���X�^���V���O��Ή�
//==========================================================
bool CModelRenderer::Draw(int mode)
{
	if (!m_pModel)return false;
	
	auto pDX = Application::Get()->GetSystem<CDXDevice>();

	XMFLOAT4X4 mtx = Transform()->GetWorldMatrix();
	
	//--- �s�����`��
	/*CLight* pLight = CLight::GetMain();
	if (!pLight)return false;*/
	
	m_pModel->Draw(pDX->GetDeviceContext(), mtx, EByOpacity(mode));

	//--- �����������`��
	//pDX->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));

	//m_pModel->Draw(pDX->GetDeviceContext(), mtx, EByOpacity::eTransparentOnly);

	//pDX->SetBlendState(static_cast<int>(EBlendState::BS_NONE));		// �����L��

	return true;
}

//==========================================================
// ���_/�C���f�b�N�X�z����
//==========================================================
void CModelRenderer::FinVertexArray()
{
	SAFE_DELETE_ARRAY(m_pIndex);
	SAFE_DELETE_ARRAY(m_pVertex);
}

//==========================================================
// ���_���ۑ�
//==========================================================
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

//==========================================================
// ���C�Ƃ̓����蔻��
// �Փ˒n�_�i�[
//==========================================================
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

//==========================================================
// �����Ƃ̓����蔻��
// �Փ˒n�_�i�[
//==========================================================
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

//==========================================================
// �����Ƃ̓����蔻��
// �Փ˒n�_�i�[(�S��)
//==========================================================
bool CModelRenderer::CollisionLineSegment(XMFLOAT3 vP0, XMFLOAT3 vP1, std::vector<Vector3>* pX)
{
	Matrix4x4 mtx = Transform()->GetWorldMatrix();
	Matrix4x4 mtxPoint;
	XMFLOAT3 pos;
	// �S�Ă̎O�p�`�ɂ��ČJ��Ԃ�
	for (UINT i = 0; i < m_nIndex; )
	{
		// �O�p�`��3���_�����݂̃}�g���b�N�X����v�Z
		XMFLOAT3 v0 = m_pVertex[m_pIndex[i++]].vPos;
		XMFLOAT3 v1 = m_pVertex[m_pIndex[i++]].vPos;
		XMFLOAT3 v2 = m_pVertex[m_pIndex[i++]].vPos;

		mtxPoint.Identity();
		mtxPoint._41 = v0.x;
		mtxPoint._42 = v0.y;
		mtxPoint._43 = v0.z;
		mtxPoint = mtxPoint.Multiply(mtx);
		pos = XMFLOAT3(mtxPoint._41, mtxPoint._42, mtxPoint._43);
		v0 = pos;

		mtxPoint.Identity();
		mtxPoint._41 = v1.x;
		mtxPoint._42 = v1.y;
		mtxPoint._43 = v1.z;
		mtxPoint = mtxPoint.Multiply(mtx);
		pos = XMFLOAT3(mtxPoint._41, mtxPoint._42, mtxPoint._43);
		v1 = pos;

		mtxPoint.Identity();
		mtxPoint._41 = v2.x;
		mtxPoint._42 = v2.y;
		mtxPoint._43 = v2.z;
		mtxPoint = mtxPoint.Multiply(mtx);
		pos = XMFLOAT3(mtxPoint._41, mtxPoint._42, mtxPoint._43);
		v2 = pos;

		XMFLOAT3& vV0 = v0;
		XMFLOAT3& vV1 = v1;
		XMFLOAT3& vV2 = v2;

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
		pX->push_back(vX);
		
		/*if (pN)
		{
			XMStoreFloat3(pN, n);
		}*/
	}
	if(pX->size() != 0)
		return true;

	return false;	// �������Ă��Ȃ�
}


#ifdef BUILD_MODE

void CModelRenderer::ImGuiDebug()
{
	using namespace MySpace::Debug;

	//--- �t�H���_����t�@�C�����擾
	if (m_aXModelList.empty() && m_aObjModelList.empty() && m_aFbxModelList.empty() || 
		ImGui::Button(u8"model reload"))
	{
		MySpace::System::CFilePath file;
		m_aXModelList = file.GetAllFileName(MODEL_PATH, ".x");
		m_aObjModelList = file.GetAllFileName(MODEL_PATH, ".obj");
		m_aFbxModelList = file.GetAllFileName(MODEL_PATH, ".fbx");
	}

	//--- ���f���I��ǂݍ���

	// x�t�@�C��
	if (auto name = DispCombo(m_aXModelList, "x File", m_modelName); !name.empty())
	{
		// �|�C���^���󂯎��
		SetModel(name);
	}
	
	// obj�t�@�C��
	if (auto name = DispCombo(m_aObjModelList, "obj File", m_modelName); !name.empty())
	{
		// �|�C���^���󂯎��
		SetModel(name);
	}
	
	// fbx�t�@�C��
	if (auto name = DispCombo(m_aFbxModelList, "fbx File", m_modelName); !name.empty())
	{
		// �|�C���^���󂯎��
		SetModel(name);
	}


	// ���O����
	m_modelName = InputString(m_modelName, u8"�ݒ胂�f��");
	if (ImGui::Button("Load"))
		SetModel(m_modelName);
	ImGui::Text("BSphere Radius :%f", GetBSRadius());
	ImGui::Checkbox("Static", (bool*)&m_nStaticMode);
	ImGui::SameLine();
	ImGui::Checkbox("Shadow", (bool*)&m_bShadow);

	//--- �}�e���A��
	ImGui::BeginTabBar("Material");
	if (ImGui::BeginTabItem("Diffuse"))
	{
		ImGui::ColorPicker4("Diffuse", (float*)&m_AssimpMaterial.Kd);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Ambient"))
	{
		ImGui::ColorPicker4("Ambient", (float*)&m_AssimpMaterial.Ka);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Specular"))
	{
		ImGui::ColorPicker4("Specular", (float*)&m_AssimpMaterial.Ks);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Emissive"))
	{
		ImGui::ColorPicker4("Emissive", (float*)&m_AssimpMaterial.Ke);
		ImGui::EndTabItem();
	}
	
	ImGui::EndTabBar();
}

#endif // BUILD_MODE