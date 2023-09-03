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
			struct STCreateWindowSet
			{
				bool bDisp;				 // 表示フラグ
				bool bCopy;				 // 複製
				float fMargin;			 // マージン
				int nGrid;				 // 縦×横
				int nObjType;			 // 複製でない場合の生成Type(cppで定義)
				MyMath::Vector3 vCenter; // 中心
				std::string strName;	 // 名前
				STCreateWindowSet():bDisp(false),bCopy(true),fMargin(1),nGrid(3),nObjType(0) 
				{}
			};
		public:
			static constexpr const char* DESC_SELECT_OBJ = u8"selectGameObjcet";	// *@ドラッグ＆ドロップ認識用文字列
		private:

			bool m_bLoadSaveWindow;
			std::string m_strSavePath;
			std::string m_strLoadPath;
			std::vector<std::string> m_aScenePathList;
			SearchSet m_Search;
			STCreateWindowSet m_CreateParam;

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
			std::shared_ptr<MySpace::Game::CGameObject> CreateObject(const int No, std::shared_ptr<MySpace::Game::CGameObject> = std::shared_ptr<MySpace::Game::CGameObject>());

			// *@クリックでオブジェクトを選択
			void ClickSelect(MySpace::Debug::ImGuiManager* manager);

		public:
			CHierachy();
			~CHierachy();

			void Update(MySpace::Debug::ImGuiManager* mgr);

			void SetPath(const std::string name) { m_strSavePath = name; };

			// *sceneファイルの再取得
			void LoadScenePathList();
		};
	}
}
#endif //BUILD_MODE

#endif // !__HIERARCHY_H__
