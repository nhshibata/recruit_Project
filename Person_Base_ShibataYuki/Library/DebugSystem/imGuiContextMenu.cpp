//=========================================================
// [imGuiContextMenu.cpp] 
// 作成:2023/03/13
//---------------------------------------------------------
// コンテキストメニュー用
// ファイル名ミスったかも
//=========================================================

//--- インクルード部
#include <DebugSystem/imGuiContextMenu.h>
#include <DebugSystem/imGuiPackage.h>

#include <GameSystem/components.h>
#include <CoreSystem/File/cerealize.h>

namespace MySpace
{

	namespace Debug
	{
		//=========================================================
		// コンテキストメニュー表示
		// FIXME:ネストが深い
		//=========================================================
		void PopUpGameObjectMenu(MySpace::Game::CGameObject* pObj)
		{
			static std::vector<std::string> menuVec = {
				u8"ParentDissolved(親子関係解消)",
				u8"Copy",
				u8"Paste",
				u8"Close",
			};

			int res = Debug::PopupMenu(menuVec, pObj->GetName());
			switch (res)
			{
				case 0:
				{
					// 親子関係解消
					auto trans = pObj->GetTransform();
					if (auto parent = trans->GetParent(); parent.lock())
					{
						trans->ParentDissolved();
					}
					break;
				}
				case 1:
				{
					// シリアライズ化して名前,ﾎﾟｲﾝﾀなどだけ上書きすればできる?
					// できた
					// シリアライズクラス作成
					CCerealize<std::shared_ptr<CGameObject>> sirial;
					{
						// ﾃﾞｰﾀを外部保存
						auto obj = pObj->GetPtr().lock();
						sirial.OutputFile(pObj->GetName(), COPY_DATA_GAME_OBJECT_PATH, obj);
					}
					break;
				}
				case 2:
				{
					CopyGameObject();
					break;
				}
			}
		}

		//=========================================================
		// オブジェクトのｺﾋﾟｰ
		//=========================================================
		std::shared_ptr<MySpace::Game::CGameObject> CopyGameObject()
		{
			// 一時的なオブジェクト生成
			auto work = std::make_shared<CGameObject>();
			
			CCerealize<std::shared_ptr<CGameObject>> sirial;
			// ﾃﾞｰﾀ読み込み
			work = sirial.InputFile(COPY_DATA_GAME_OBJECT_PATH);
			if (!work)
				return work;

			// 新しいオブジェクト生成
			auto newObj = CGameObject::CreateObject();
			// 情報設定
			newObj.lock()->SetName(work->GetName() + "_Clone");
			newObj.lock()->SetObjTag(work->GetTag());
			newObj.lock()->SetLayer(work->GetLayer());
			// 読みこまれたコンポーネントの受け渡し
			auto comList = work->GetComponentList();
			for (auto & com : comList)
			{
				newObj.lock()->SetComponent(com);
				//--- 描画と当たり判定クラスは要請する必要があるため、Initを呼び出す
				// NOTE: 限定的なもので汎用性に欠ける。正直どうなのか
				if (com->GetName().find("Renderer") != std::string::npos ||
					com->GetName().find("Collision") != std::string::npos)
				{
					com->Awake();
					com->Init();
				}
			}
			// オブジェクト破棄
			work.reset();

			return newObj.lock();
		}

	}

}
