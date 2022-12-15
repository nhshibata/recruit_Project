//=====================================================
// [imGuiSceneGizmo.h]
// �쐬:2022/11/07
// 
// ------------------------
// ImGui��Gizmo���g�p
// �X�e�[�W�쐬�p�̃I�u�W�F�N�g�̐ݒu�ɗ~��������
// �ʃv���W�F�N�g�Ŏ����Ă���ڐA
//=====================================================

//--- �C���N���[�h�K�[�h
#ifndef __IMGUI_SCENE_GIZMO_H__
#define __IMGUI_SCENE_GIZMO_H__

//--- �C���N���[�h��
#include <ImGuizmo/ImGuizmo.h>
#include <CoreSystem/Math/myVector.h>

#pragma region ForwardDeclaration
namespace MySpace
{
	namespace Game
	{
		class CCamera;
		class CTransform;
	}

	namespace Debug
	{
		class ImGuiManager;
	}
}
#pragma endregion

namespace MySpace
{
	namespace Debug
	{
		//--- �O���Q��
		using MySpace::Game::CCamera;
		using MySpace::Game::CTransform;

		//--- �N���X��`
		class CMyGizmo
		{
		private:
			//--- �����o�ϐ�
			ImGuizmo::OPERATION m_CurrentGizmoOperation;				// �ړ��E��]�E�g�k���
			ImGuizmo::MODE m_CurrentGizmoMode;							// ���[�J�� or ���[���h
			bool m_bUseSnap = true;										// snap���
			MyMath::Vector3 m_vSnapMove = { 1,1,1 };					// snap���̕ύX��
			float m_aBounds[6] = { -0.5f,-0.5f,-0.5f,0.5f,0.5f,0.5f };
			float m_aBoundsSnapMove[3] = { 0.1f,0.1f,0.1f };			// snap���ł͂Ȃ����̈ړ���
			bool m_bBoundSizing = false;
			bool m_bBoundSizingSnap = false;

		public:
			//--- �����o�֐�
			void Init();
			void ViewGizmo(MySpace::Debug::ImGuiManager* manager, const CCamera& camera, CTransform* editObj);
			void EditTransform(MySpace::Debug::ImGuiManager* manager);
		};
	}
}

#endif // !__IMGUI_SCENE_GIZMO_H__
