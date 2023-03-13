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
#include <CoreSystem/Math/myVector.h>

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
				MAX
			};
			//--- 構造体定義
			struct SearchSet				// 検索セット
			{
				ESearchTerms eTerms = ESearchTerms::OBJ_NAME;
				bool bSearchCriteria = false;
				std::string inputName;
			};
			struct CreateWindowSet
			{
				bool bDisp;
				float fMargin;
				int nGrid;
				int nObjType;
				MyMath::Vector3 vCenter;
				CreateWindowSet():bDisp(false),fMargin(1),nGrid(3),nObjType(0) {}
			};
		private:
			static constexpr const char* DESC_SELECT_OBJ = u8"selectGameObjcet";	// *@ドラッグ＆ドロップ認識用文字列

			bool m_bLoadSaveWindow;
			std::string m_strSavePath;
			std::string m_strLoadPath;
			std::vector<std::string> m_aScenePathList;
			SearchSet m_Search;
			CreateWindowSet m_CreateValue;

		private:
			//--- メンバ関数
			// *@セーブとロード表示
			void DispSaveLoadMenu();
			
			// *@検索更新
			void DispSearch();
			
			// *@検索条件と一致するか確認する
			bool DispCheck(MySpace::Game::CGameObject* obj);
			
			// *@子要素の表示
			// *@孫の表示などを行うため、再帰する
			void DispChild(MySpace::Debug::ImGuiManager* manager, std::weak_ptr<MySpace::Game::CGameObject> obj);

			// *@オブジェクト一括生成
			void CreateObjectsWindow();
			std::shared_ptr<MySpace::Game::CGameObject> CreateObject(int No, std::shared_ptr<MySpace::Game::CGameObject> = std::shared_ptr<MySpace::Game::CGameObject>());


		public:
			CHierachy();
			~CHierachy();

			void Update(MySpace::Debug::ImGuiManager* mgr);

			void SetPath(std::string name) { m_strSavePath = name; };

			// *sceneファイルの再取得
			void LoadScenePathList();
		};
	}
}
#endif //BUILD_MODE

#endif // !__HIERARCHY_H__
