//==========================================================
// [DrawManager.h]
// �h���N���X
// �쐬:2022/06/07
// �X�V:2022/09/11 �����䓖���蔻��𒲐�
// �׽���ύX���邩��
//==========================================================

//--- �C���N���[�h��
#include <algorithm>
#include <ImGui/imgui.h>
#include <GameSystem/Manager/DrawManager.h>
#include <GameSystem/Component/Camera/camera.h>

#include <GraphicsSystem/Manager/imageResourceManager.h>
#include <GraphicsSystem/Manager/effectManager.h>
#include <GraphicsSystem/Manager/modelManager.h>

using namespace MySpace::Game;

namespace 
{
#ifdef BUILD_MODE
	static int g_nSkipCnt;
	static int g_nDrawCnt;
#endif // _DEBUG
}

CDrawManager::CDrawManager()
	:m_bIsSort(false)
{
}
CDrawManager::~CDrawManager()
{
	// �������
	CImageResourceManager::Get().Uninit();
	CEffekseer::Get().Uninit();
	CModelManager::Get().Uninit();
}
void CDrawManager::Update()
{
	// CRenderer::Draw��bool��Ԃ�
	// true�ł���Ε`�悷��
	// �`�搔�Ȃǂ̊m�F������Ȃ�΂�������p����
	
	// ���C���[�ŕ��ёւ�
	//std::sort(m_ObjectList.begin(), m_ObjectList.end(), [](std::weak_ptr<CRenderer> s1, std::weak_ptr<CRenderer> s2)
	if (m_bIsSort)
	{
		// �`��o���Ȃ����̂͏��O
		EraseDraw();
		// ����
		//m_pObjectList.back().lock()->;
		std::sort(m_pSortList.begin(), m_pSortList.end(), [](auto const& s1, auto const& s2)->bool
		{
			return s1.lock()->GetLayer() < s2.lock()->GetLayer();
		});
		m_bIsSort = false;

#ifdef _DEBUG
		/*std::vector<int> check;
		for (auto & render : m_pSortList)
		{
			check.push_back(render.lock()->GetOwner()->GetLayer());

		}*/
#endif // _DEBUG

	}
	
	int nEeraseCnt = 0;

	// 3D�̕`��
	for (auto & render : m_pSortList)
	{
		// �|�C���^�m�F
		if (!render.lock())
		{
			++nEeraseCnt;
			continue;		// null
		}

		// �`��\�ȏ�Ԃ��m�F
		if (!render.lock()->IsActive())
			continue;

#ifdef BUILD_MODE
		++g_nDrawCnt;
		auto name = render.lock()->GetName();
#endif // _DEBUG

		// Mesh���߰��Ă��m�F
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
				++g_nSkipCnt;
#endif // _DEBUG
				continue;
			}
		}

		// �`��
		render.lock()->Draw();
	}

	// �����͓K��
	if (nEeraseCnt > 5)
	{
		EraseDraw();
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

void CDrawManager::ImGuiDebug()
{
	ImGui::Text(u8"DrawManager");
	ImGui::Text(u8"�`�惊�X�g�� %d", m_pSortList.size());
#ifdef BUILD_MODE
	ImGui::Text(u8"�`�搔 %d", g_nDrawCnt);
	ImGui::SameLine();
	ImGui::Text(u8"�`��X�L�b�v�� %d", g_nSkipCnt);
	
	g_nSkipCnt = 0;
	g_nDrawCnt = 0;
#endif // _DEBUG
	ImGui::Checkbox("�`��\�[�g", &m_bIsSort);
}
#endif