//=====================================================
// [imGuiSceneGizmo.h]
// 作成:2022/11/07
// 
// ------------------------
// ImGuiのGizmoを使用
// ステージ作成用のオブジェクトの設置に欲しかった
// 別プロジェクトで試してから移植
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
		//--- 前方参照
		using MySpace::Game::CCamera;

		//--- クラス定義
		class CMyGizmo
		{
		private:
			ImGuizmo::OPERATION m_CurrentGizmoOperation;		// 移動・回転・拡縮状態
			ImGuizmo::MODE m_CurrentGizmoMode;					// ローカル or ワールド
			bool m_bUseSnap = true;													// snap状態
			MyMath::Vector3 m_vSnapMove = { 1,1,1 };												// snap時の変更量
			float m_aBounds[6] = { -0.5f,-0.5f,-0.5f,0.5f,0.5f,0.5f };
			float m_aBoundsSnapMove[3] = { 0.1f,0.1f,0.1f };								// snap時ではない時の移動量
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
