//=========================================================
// [inspector.h] 
// 作成:2022/07/10
//---------------------------------------------------------
//=========================================================

//--- インクルードガード
#ifndef __INSPECTOR_H__
#define __INSPECTOR_H__

//--- インクルード部
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
			//--- メンバ関数
			void DispDebugSelectObject();		// *@選択中Objectの表示
			void AddComponentWindow();			// *@選択中Objectのウィンドウ表示
			void DeleteObject();				// *@削除
			void CopyGameObject();				// *@オブジェクトのｺﾋﾟｰ
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