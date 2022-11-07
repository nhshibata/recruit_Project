//=====================================================
// [imGuiSceneGizmo.h]
// �쐬:2022/11/07
// 
// ------------------------
// ImGui��Gizmo���g�p
// �X�e�[�W�쐬�p�̃I�u�W�F�N�g�̐ݒu�ɗ~��������
// �ʃv���W�F�N�g�Ŏ����Ă���ڐA
//=====================================================

#ifndef __IMGUI_SCENE_GIZMO_H__
#define __IMGUI_SCENE_GIZMO_H__

#include <ImGuizmo/ImGuizmo.h>
#include <CoreSystem/Math/myVector.h>

namespace MySpace
{
	namespace Game
	{
		class CCamera;
	}
}

namespace MySpace
{
	namespace Debug
	{
		//--- �O���Q��
		using MySpace::Game::CCamera;

		//--- �N���X��`
		class CMyGizmo
		{
		private:
			ImGuizmo::OPERATION m_CurrentGizmoOperation;		// �ړ��E��]�E�g�k���
			ImGuizmo::MODE m_CurrentGizmoMode;					// ���[�J�� or ���[���h
			bool m_bUseSnap = true;													// snap���
			MyMath::Vector3 m_vSnapMove = { 1,1,1 };												// snap���̕ύX��
			float m_aBounds[6] = { -0.5f,-0.5f,-0.5f,0.5f,0.5f,0.5f };
			float m_aBoundsSnapMove[3] = { 0.1f,0.1f,0.1f };								// snap���ł͂Ȃ����̈ړ���
			bool m_bBoundSizing = false;
			bool m_bBoundSizingSnap = false;
		public:
			CMyGizmo() {
				m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
				m_CurrentGizmoMode = ImGuizmo::LOCAL;
			}
			~CMyGizmo() {};

			void EditTransform(const CCamera& camera, MyMath::Matrix4x4& matrix);
		};
	}
}

#endif // !__IMGUI_SCENE_GIZMO_H__
