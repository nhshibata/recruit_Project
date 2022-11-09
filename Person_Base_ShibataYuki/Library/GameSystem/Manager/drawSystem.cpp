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
	//std::sort(m_ObjectList.begin(), m_ObjectList.end(), [](std::weak_ptr<CRenderer> s1, std::weak_ptr<CRenderer> s2)
	if (m_bIsSortNecessary)
	{
		// ����
		//m_pObjectList.back().lock()->;
		std::sort(m_pDrawSortList.begin(), m_pDrawSortList.end(), [](auto const& s1, auto const& s2)->bool
		{
			return s1.lock()->GetLayer() < s2.lock()->GetLayer();
		});

		m_bIsSortNecessary = false;

#ifdef _DEBUG
		/*std::vector<int> check;
		for (auto & render : m_pSortList)
		{
			check.push_back(render.lock()->GetOwner()->GetLayer());

		}*/
#endif // _DEBUG

	}
	
	// 3D�̕`��
	for (auto & render : m_pDrawSortList)
	{
		// �|�C���^�m�F
		if (!render.lock())
		{
			continue;
		}

		// �`��\�ȏ�Ԃ��m�F
		if (!render.lock()->IsActive())
			continue;

#ifdef BUILD_MODE
		++m_nDrawCnt;
		auto name = render.lock()->GetName();
#endif // _DEBUG

		// Mesh���߰���(����ьp��)���m�F
		// TODO: �v�����s���S
		if (auto mesh = render.lock()->BaseToDerived<CMeshRenderer>().get(); mesh)
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
	ImGui::Text(u8"DrawManager");
	ImGui::Text(u8"�`�惊�X�g�� %d", m_pDrawSortList.size());
	ImGui::Text(u8"�`�搔 %d", m_nDrawCnt);
	ImGui::SameLine();
	ImGui::Text(u8"�`��X�L�b�v�� %d", m_nSkipCnt);
	
	m_nDrawCnt = 0;
	m_nSkipCnt = 0;
	ImGui::Checkbox(u8"�`��\�[�g", &m_bIsSortNecessary);
}
#endif