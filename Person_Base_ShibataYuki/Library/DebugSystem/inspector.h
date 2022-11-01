//=========================================================
// [inspector.h] 
// 作成:2022/07/10
//---------------------------------------------------------
//=========================================================

//--- インクルードガード
#ifndef __INSPECTOR_H__
#define __INSPECTOR_H__

//--- インクルード部
#include <DebugSystem/debug.h>
#include <vector>
#include <memory>
#include <CoreSystem/Math/myVector.h>

#if BUILD_MODE

#pragma region forward_declaration
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
			enum class EObjMoveType : int
			{
				POSITION = 0,
				ROTATE,
				SCALE,
				MAX
			};
			struct STTransformSet
			{
				EObjMoveType eType = EObjMoveType::POSITION;
				float fMoveValue = 1.0f;
				std::weak_ptr<CRectTransform> spRect;
			};
		private:
			std::weak_ptr<CGameObject> m_spViewObj;
			std::vector<bool> m_isDrawInfo;
			bool m_isComponent;
			bool m_isDeleted;
			//bool m_bDispObj;
			STTransformSet m_TransformController;

		private:
			//--- メンバ関数
			void DebugObject();
			void MoveObject();
			void AddComponentWindow();
			void DeleteObject();
			void DeleteInformation();
			void Copy();
			void Value(float* value, MySpace::MyMath::Vector3* vec);
			void Value(float* value, MySpace::MyMath::Vector2* vec);
			void Value(float* value, float vec);
		public:
			CInspector();
			~CInspector();

			void Init();
			void Uninit();
			void Update();
			void Draw();

			void SetGameObject(std::weak_ptr<CGameObject> obj);
			//void SetDispObj(bool flg) { m_bDispObj = flg; }
		};
	}
}
#endif // BUILD_MODE

#endif	//__INSPECTOR_H__