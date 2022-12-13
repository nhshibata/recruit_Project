//==========================================================
// [drawSystem.cpp]
// �h���N���X
//---------------
// �쐬:2022/06/07 �׽���ύX���邩��
// �X�V:2022/09/11 �����䓖���蔻��𒲐�
// �X�V:2022/11/09 �N���X���ύX(DrawManager) -> (drawSystem)
//==========================================================

//--- �C���N���[�h��
#include <algorithm>
#include <ImGui/imgui.h>
#include <GameSystem/Manager/drawSystem.h>
#include <GameSystem/Component/Camera/camera.h>

#include <GraphicsSystem/Manager/imageResourceManager.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>
#include <GameSystem/Component/Renderer/meshRenderer.h>
#include <GraphicsSystem/Manager/effectManager.h>
#include <GraphicsSystem/Manager/modelManager.h>
#include <GraphicsSystem/Render/billboard.h>
#include <GraphicsSystem/Manager/imageResourceManager.h>


using namespace MySpace::Game;

CDrawSystem::CDrawSystem()
	:m_bIsSortNecessary(false)
{
}
CDrawSystem::~CDrawSystem()
{
	// �������
	CImageResourceManager::Get().Uninit();
	CEffekseer::Get().Uninit();
	CModelManager::Get().Uninit();
}
void CDrawSystem::Update()
{
	// CRenderer::Draw��bool��Ԃ�
	// true�ł���Ε`�悷��
	// �`�搔�Ȃǂ̊m�F������Ȃ�΂�������p����
	
	// ���C���[�ŕ��ёւ�
	if (m_bIsSortNecessary)
	{
		// ����
		//m_pObjectList.back().lock()->;
		std::sort(m_pDrawSortList.begin(), m_pDrawSortList.end(), [](auto const& s1, auto const& s2)->bool
		{
			return s1.lock()->GetLayer() < s2.lock()->GetLayer();
		});

		m_bIsSortNecessary = false;
	}
	
	// 3D�̕`��
	for (auto & render : m_pDrawSortList)
	{
		// �|�C���^�m�F
		if (!render.lock())
			continue;

		// �`��\�ȏ�Ԃ��m�F
		if (!render.lock()->IsActive())
			continue;

#ifdef BUILD_MODE
		++m_nDrawCnt;
		auto name = render.lock()->GetName();
#endif // _DEBUG

		// Mesh���߰���(����ьp��)���m�F
		if (auto mesh = render.lock()->BaseToDerived<CMeshRenderer>().get(); mesh && m_bFrustum)
		{	
			float fRadius = 0.0f;
			auto mW = mesh->Transform()->GetWorldMatrix();
			fRadius = (mW._11 > fRadius) ? mW._11 : fRadius;
			fRadius = (mW._22 > fRadius) ? mW._22 : fRadius;
			fRadius = (mW._33 > fRadius) ? mW._33 : fRadius;
			
			// ��ׂɉf�邩����
			//if (CCamera::GetMain()->CollisionViewFrustum(&mesh->GetCenter(0), mesh->GetBSRadius()) == CCamera::EFrustumResult::OUTSIDE)
			if (CCamera::GetMain()->CollisionViewFrustum(&Vector3(mW._41, mW._42 ,mW._43), fRadius) == CCamera::EFrustumResult::OUTSIDE)
			{
#ifdef BUILD_MODE
				++m_nSkipCnt;
#endif // _DEBUG
				continue;
			}
		}

		// �`��
		render.lock()->Draw();
	}

	//--- �C���X�^���V���O�`��
	CLight* pLight = CLight::Get();
	pLight->SetDisable(false);	// ���C�e�B���O����
	for (auto & draw : m_aInstancingModelMap)
	{
		auto model = CModelManager::Get().GetModel(draw.first);
		// 100�ȏ�̎�
		// �C���X�^���V���O�ɂ�100�����̈���m�ۂ��Ă��Ȃ����߁A100�ȏ�̏ꍇ�͈�U��؂�
		if (draw.second.size() >= 100)
		{
			std::vector<DirectX::XMFLOAT4X4> inData;
			for (int idxCnt = 0; idxCnt < static_cast<int>(draw.second.size()); ++idxCnt)
			{
				int idx = idxCnt % 100;
				if (idxCnt != 0 && idx == 100-1)
				{
					model->DrawInstancing(CDXDevice::Get().GetDeviceContext(), inData);
					inData.clear();
					continue;
				}
				inData[idx] = draw.second[idx];
			}
			// ����
			draw.second.clear();
			continue;
		}

		model->DrawInstancing(CDXDevice::Get().GetDeviceContext(), draw.second);
		draw.second.clear();

#if _DEBUG
		++m_nInstancingCnt;
#endif // _DEBUG

	}
	// �N���A
	m_aInstancingModelMap.clear();

	//--- ���b�V���C���X�^���V���O
	int cnt = 0;
	for (auto & mesh : m_aInstancingMesh)
	{
		if (mesh.second.size() == 0)// �ꉞ�m�F
			continue;
#if _DEBUG
		++m_nInstancingCnt;
#endif // _DEBUG

		// �r���{�[�h���m�F
		CBillboard* bill = dynamic_cast<CBillboard*>(mesh.second[0]);
		if (bill)
		{
			auto image = CImageResourceManager::Get().GetResource(mesh.first);
			auto tex = image ? image->GetSRV() : NULL;
			CMesh::DrawInstancing(mesh.second, tex, &bill->GetTextureMatrix());
		}
		else
		{
			CMesh::DrawInstancing(mesh.second);
		}
	}
	m_aInstancingMesh.clear();	// �N���A

	pLight->SetEnable();		// ���C�e�B���O�L��
	CDXDevice::Get().SetZBuffer(true);			
	//CDXDevice::Get().SetBlendState(static_cast<int>(EBlendState::BS_NONE));		// ���u�����f�B���O����


#if 0
	if (m_bIsSort)
	{
		std::sort(m_ObjectList.begin(), m_ObjectList.end(), [](auto const& s1, auto const& s2)->bool
		{
			/*if (!s1.lock()->IsActive())
				activ.push_back(s1.lock());*/
			auto name = s1.lock()->GetName();
			if (s1.lock() && s2.lock())
				return false;
			//return s1.lock()->GetOwner() > s2.lock()->GetOwner();
			return s1.lock()->GetOwner()->GetLayer() < s2.lock()->GetOwner()->GetLayer();
		});
		m_bIsSort = false;
	}

	for (auto & render : m_ObjectList)
	{
		// �|�C���^�m�F
		if(!render.lock())
		{
			continue;		// null
		}

		// �`��\�ȏ�Ԃ��m�F
		if (!render.lock()->IsActive())
			continue;

#ifdef _DEBUG
		++cnt;
		auto name = render.lock()->GetName();
#endif // _DEBUG

		// Mesh���߰��Ă��m�F
		// ��ׂɉf�邩����
		if (auto mesh = render.lock()->BaseToDerived<CMeshRenderer>().get(); mesh)
		{
			if (!CCamera::GetMain()->CollisionViewFrustum(&mesh->GetCenter(), mesh->GetBSRadius()))
			{
				continue;
			}
		}

		// �`��
		render.lock()->Draw();
	}
#endif // 0

}

#ifdef BUILD_MODE

void CDrawSystem::ImGuiDebug()
{
	/*if (!ImGui::TreeNode("---Draw---"))
		return;*/
	ImGui::Text(u8"---Draw---");
	ImGui::Text(u8"�`�惊�X�g�� : %d", m_pDrawSortList.size());
	ImGui::Text(u8"�`��OK�� : %d", m_nDrawCnt);
	ImGui::SameLine();
	ImGui::Text(u8"�`��X�L�b�v�� : %d", m_nSkipCnt);
	ImGui::SameLine();
	ImGui::Checkbox("ON/OFF", (bool*)&m_bFrustum);
	ImGui::Text(u8"�C���X�^���V���O�� : %d", m_nInstancingCnt);
	ImGui::Checkbox(u8"�`��\�[�gON", &m_bIsSortNecessary);

	ImGui::Text("Resource/Model:%d", CModelManager::Get().GetNameList().size());
	ImGui::Text("Resource/Image:%d", CImageResourceManager::Get().GetNameList().size());
	ImGui::Text("Resource/Effekseer:%d", CEffekseer::Get().GetNameList().size());

	//ImGui::TreePop();
	//--- ���̃t���[���p������
	m_nDrawCnt = 0;
	m_nSkipCnt = 0;
	m_nInstancingCnt = 0;
}
#endif