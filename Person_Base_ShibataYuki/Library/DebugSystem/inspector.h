//=========================================================
// [inspector.h] 
// �쐬:2022/07/10
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __INSPECTOR_H__
#define __INSPECTOR_H__

//--- �C���N���[�h��
#include <vector>
#include <memory>
#include <CoreSystem/Math/myVector.h>

#if BUILD_MODE

#pragma region ForwardDeclaration
namespace MySpace 
{
	namespace MyMath
	{
		class Vector3;
		class Vector2;
	}

	namespace Game
	{
		class CGameObject;
		class CRectTransform;
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
		using namespace MySpace::Game;

		class CInspector
		{
		private:
			std::weak_ptr<CGameObject> m_spViewObj;
			std::vector<bool> m_isDrawInfo;
			bool m_isComponent;
			bool m_isDeleted;
			bool m_bOpen;
			//bool m_bDispObj;
			//STTransformSet m_TransformController;

		private:
			//--- �����o�֐�
			void DispDebugSelectObject();		// *@�I��Object�̕\��
			void AddComponentWindow();			// *@�I��Object�̃E�B���h�E�\��
			void DeleteObject();				// *@�폜
			void CopyGameObject();				// *@�I�u�W�F�N�g�̺�߰
			void DispPopUpMenuObject();
			//void MoveObject();
			//void Value(float* value, MySpace::MyMath::Vector3* vec);
			//void Value(float* value, MySpace::MyMath::Vector2* vec);
			//void Value(float* value, float vec);
		public:
			CInspector();
			~CInspector();

			void Update(MySpace::Debug::ImGuiManager*);

			_NODISCARD inline std::weak_ptr<CGameObject> GetSelectObject() { return  m_spViewObj; }
			void SetSelectGameObject(std::weak_ptr<CGameObject> obj);
			//void SetDispObj(bool flg) { m_bDispObj = flg; }
		};
	}
}
#endif // BUILD_MODE

#endif	//__INSPECTOR_H__