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
	:m_bIsSortNecessary(true), m_bFrustum(true)
{
	m_pDrawSortList.clear();
	m_aInstancingModelMap.clear();
	m_aInstancingMesh.clear();
	
#if BUILD_MODE
	m_nSkipCnt = m_nDrawCnt = m_nInstancingCnt = 0;
#endif // BUILD_MODE


}
CDrawSystem::~CDrawSystem()
{
	// �������
	CImageResourceManager::Get()->Uninit();
	CEffekseer::Get()->Uninit();
	CModelManager::Get()->Uninit();
}
void CDrawSystem::Update()
{
	// CRenderer::Draw��bool��Ԃ�
	// true�ł���Ε`�悷��
	// �`�搔�Ȃǂ̊m�F������Ȃ�΂�������p����
	
	//--- ���C���[�ŕ��ёւ�
	// �ǉ����ꂽ�Ƃ��Ȃ�
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
	
	//--- �`��
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

		//--- Mesh���߰���(����ьp��)���m�F
		// �J�����O�t���O��ON�����m�F
		if (auto mesh = render.lock()->BaseToDerived<CMeshRenderer>().get(); mesh && m_bFrustum)
		{	
			float fRadius = mesh->GetBSRadius();
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

		//--- �����܂ł�����`��
		render.lock()->Draw();
	}

	//--- �C���X�^���V���O�`��
	CLight* pLight = CLight::Get();
	pLight->SetDisable(false);			// ���C�e�B���O����
	//--- �o�^���ꂽ���O�ʂɕ`��
	const int MAX_INSTANCING = 100;
	for (auto & draw : m_aInstancingModelMap)
	{
		auto model = CModelManager::Get()->GetModel(draw.first);
		//--- �萔�ȏ�̎�
		// �C���X�^���V���O�ɂ͒萔�������̈���m�ۂ��Ă��Ȃ����߁A���̒l�ȏ�̏ꍇ�͈�U��؂�
		if (draw.second.size() >= MAX_INSTANCING)
		{
			std::vector<DirectX::XMFLOAT4X4> inData;
			for (int idxCnt = 0; idxCnt < static_cast<int>(draw.second.size()); ++idxCnt)
			{
				int idx = idxCnt % MAX_INSTANCING;
				if (idxCnt != 0 && idx == MAX_INSTANCING - 1)
				{
					model->DrawInstancing(CDXDevice::Get()->GetDeviceContext(), inData);
					inData.clear();
					continue;
				}
				inData[idx] = draw.second[idx];
			}
			// ���̃��f����
			draw.second.clear();
			continue;
		}

		model->DrawInstancing(CDXDevice::Get()->GetDeviceContext(), draw.second);
		draw.second.clear();

#if BUILD_MODE
		++m_nInstancingCnt;
#endif // _DEBUG

	}
	// �N���A
	m_aInstancingModelMap.clear();

	//--- ���b�V���C���X�^���V���O�`��
	for (auto & mesh : m_aInstancingMesh)
	{
		if (mesh.second.size() == 0)// �ꉞ�m�F
			continue;
#if BUILD_MODE
		++m_nInstancingCnt;
#endif // _DEBUG

		// �r���{�[�h���m�F
		CBillboard* bill = dynamic_cast<CBillboard*>(mesh.second[0]);
		if (bill)
		{
			auto image = CImageResourceManager::Get()->GetResource(mesh.first);
			auto tex = image ? image->GetSRV() : NULL;
			CMesh::DrawInstancing(mesh.second, tex, &bill->GetTextureMatrix());
		}
		else
		{
			CMesh::DrawInstancing(mesh.second);
		}
	}
	m_aInstancingMesh.clear();	// �N���A

	//--- �ݒ��߂�
	pLight->SetEnable();		// ���C�e�B���O�L��
	CDXDevice::Get()->SetZBuffer(true);			
	//CDXDevice::Get()->SetBlendState(static_cast<int>(EBlendState::BS_NONE));		// ���u�����f�B���O����


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
	ImGui::Text(u8"�`�惊�X�g�� : %d", m_pDrawSortList.size());
	ImGui::Text(u8"�`��OK�� : %d", m_nDrawCnt);
	ImGui::SameLine();
	ImGui::Text(u8"�`��X�L�b�v�� : %d", m_nSkipCnt);
	ImGui::SameLine();
	ImGui::Checkbox("Culling ON/OFF", (bool*)&m_bFrustum);
	ImGui::Text(u8"�C���X�^���V���O�� : %d", m_nInstancingCnt);
	ImGui::Checkbox(u8"�`��\�[�gON", &m_bIsSortNecessary);

	ImGui::Text("Resource/Model:%d", CModelManager::Get()->GetNameList().size());
	ImGui::Text("Resource/Image:%d", CImageResourceManager::Get()->GetNameList().size());
	ImGui::Text("Resource/Effekseer:%d", CEffekseer::Get()->GetNameList().size());

	//--- ���̃t���[���p������
	m_nDrawCnt = 0;
	m_nSkipCnt = 0;
	m_nInstancingCnt = 0;
}
#endif