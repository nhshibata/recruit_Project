//=========================================================
// [hirarchy.h] 
// 作成:2022/07/10
//---------------------------------------------------------
//=========================================================

//--- インクルードガード
#ifndef __HIERARCHY_H__
#define __HIERARCHY_H__

//--- インクルード部
#include <CoreSystem/Util/stl.h>
#include <DebugSystem/debug.h>

#ifdef BUILD_MODE

#pragma region forward_declaration
namespace MySpace
{
	namespace Game
	{
		class CGameObject;
	}

}
#pragma endregion
		
namespace MySpace
{
	namespace Debug
	{

		class CHierachy
		{
		private:
			enum class ESearchTerms : int
			{
				OBJ_NAME,
				TAG,
				STATE_ACTIVE,
				STATE_STOP,
				STATE_DESTROY,
				MAX
			};
			struct SearchSet
			{
				ESearchTerms eTerms = ESearchTerms::OBJ_NAME;
				bool bSearchCriteria = false;
				std::string inputName;
			};
		private:
			static constexpr const char* DESC_SELECT_OBJ = u8"selectGameObjcet";

			bool m_bLoadSaveWindow;
			SearchSet m_Search;
			std::string m_savePath;
			std::string m_loadPath;
			std::vector<std::string> m_scenePathList;

		private:
			//--- メンバ関数
			void DispSaveLoadMenu();
			void UpdateSearch();
			bool DispCheck(MySpace::Game::CGameObject* obj);
			template<class T>
			_NODISCARD std::list<T> MovingInList(std::list<T> list, T newT, int index);
			void DispChild(std::weak_ptr<MySpace::Game::CGameObject> obj);


		public:
			CHierachy();
			~CHierachy();

			void Init();
			void Uninit();
			void Update();

			void SetPath(std::string name) { m_savePath = name; };
			// *sceneファイルの再取得
			void LoadScenePathList();
		};
	}
}
#endif //BUILD_MODE

#endif // !__HIERARCHY_H__
