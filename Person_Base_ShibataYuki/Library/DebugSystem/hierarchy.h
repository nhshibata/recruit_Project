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

#if BUILD_MODE

#pragma region forward_declaration
namespace MySpace
{
	namespace Game
	{
		class CGameObject;
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
		class CHierachy
		{
		private:
			//--- 列挙体定義
			enum class ESearchTerms : int	// 検索条件
			{
				OBJ_NAME,
				TAG,
				COMPONENT,
				STATE_ACTIVE,
				STATE_STOP,
				STATE_DESTROY,
				MAX
			};
			//--- 構造体定義
			struct SearchSet				// 検索セット
			{
				ESearchTerms eTerms = ESearchTerms::OBJ_NAME;
				bool bSearchCriteria = false;
				std::string inputName;
			};
		private:
			static constexpr const char* DESC_SELECT_OBJ = u8"selectGameObjcet";	// *@ドラッグ＆ドロップ認識用文字列

			bool m_bLoadSaveWindow;
			SearchSet m_Search;
			std::string m_savePath;
			std::string m_loadPath;
			std::vector<std::string> m_scenePathList;

		private:
			//--- メンバ関数
			// *@セーブとロード表示
			void DispSaveLoadMenu();
			// *@検索更新
			void DispSearch();
			// *@検索条件と一致するか確認する
			bool DispCheck(MySpace::Game::CGameObject* obj);
			// *@リスト内の移動用関数
			template<class T>
			_NODISCARD std::list<T> MovingInList(std::list<T> list, T newT, int index);
			// *@子要素の表示
			// *@孫の表示などを行うため、再帰する
			void DispChild(MySpace::Debug::ImGuiManager* manager, std::weak_ptr<MySpace::Game::CGameObject> obj);

		public:
			CHierachy();
			~CHierachy();

			void Init();
			void Uninit();
			void Update(MySpace::Debug::ImGuiManager*);

			void SetPath(std::string name) { m_savePath = name; };
			// *sceneファイルの再取得
			void LoadScenePathList();
		};
	}
}
#endif //BUILD_MODE

#endif // !__HIERARCHY_H__
