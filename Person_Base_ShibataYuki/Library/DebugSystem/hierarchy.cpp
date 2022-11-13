//=========================================================
// [hirarchy.cpp] 
// 作成:2022/07/10
//---------------------------------------------------------
//=========================================================


//--- インクルード部
#include <DebugSystem/hierarchy.h>
#if BUILD_MODE

#include <DebugSystem/imguiManager.h>
#include <DebugSystem/inspector.h>
#include <DebugSystem/imGuiPackage.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_win32.h>
#include <ImGui/imgui_impl_dx11.h>

#include <GameSystem/Scene/scene.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Component/Camera/debugCamera.h>
#include <GameSystem/Component/Renderer/billboardRenderer.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>
#include <GameSystem/Component/Renderer/textRenderer.h>
#include <GameSystem/Component/Renderer/sphereRenderer.h>
#include <GameSystem/Component/Renderer/boxRenderer.h>
#include <GameSystem/Component/Renderer/effekseerRenderer.h>
#include <GameSystem/Component/Renderer/modelRenderer.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/gameObjectManager.h>

#include <CoreSystem/File/filePath.h>

using namespace MySpace::System;
using namespace MySpace::Debug;
using namespace MySpace::SceneManager;
using namespace MySpace::Game;

CHierachy::CHierachy()
	:m_bLoadSaveWindow(false)
{
	m_loadPath = FORDER_DIR(Data/scene);
	LoadScenePathList();
}
CHierachy::~CHierachy()
{
}
void CHierachy::Init()
{
	LoadScenePathList();
}
void CHierachy::Uninit()
{
}
void CHierachy::Update()
{
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200, 400), ImGuiCond_Once);
	bool flg = true;
	ImGui::Begin(u8"Hierarchy", &flg, ImGuiWindowFlags_MenuBar);

	if (!CSceneManager::Get().GetActiveScene())
		return;

	if (ImGui::IsWindowHovered())
	{
		ImGuiManager::Get().UpHover(ImGuiManager::EIsHovered::HOVERED_WINDOW);
	}

	auto objList = CSceneManager::Get().GetActiveScene()->GetObjManager()->GetList();

	if (ImGui::BeginMenuBar())
	{
		// ファイル
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene"))
			{
				CSceneManager::Get().CreateNewScene<CScene>();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Load/Save"))
			{
				m_bLoadSaveWindow = !m_bLoadSaveWindow;
			}
			ImGui::EndMenu();
		}
		// オブジェクト生成
		if (ImGui::BeginMenu("GameObject"))
		{
			if (ImGui::MenuItem("Empty"))
			{
				auto obj = CSceneManager::Get().GetActiveScene()->GetObjManager()->CreateGameObject();
				ImGuiManager::Get().GetInspector()->SetGameObject(obj);
			}
			if (ImGui::MenuItem("Model"))
			{
				auto obj = CGameObject::CreateObject().lock();
				obj->AddComponent<CModelRenderer>();
				// ｺﾝﾎﾟｰﾈﾝﾄの追加後
				ImGuiManager::Get().GetInspector()->SetGameObject(obj);
			}
			if (ImGui::MenuItem("Billboard"))
			{
				auto obj = CGameObject::CreateObject().lock();
				obj->AddComponent<Game::CBillboardRenderer>();
				// ｺﾝﾎﾟｰﾈﾝﾄの追加後
				ImGuiManager::Get().GetInspector()->SetGameObject(obj);
			}
			if (ImGui::MenuItem("Sphere"))
			{
				auto obj = CGameObject::CreateObject().lock();
				obj->AddComponent<Game::CSphereRenderer>();
				// ｺﾝﾎﾟｰﾈﾝﾄの追加後
				ImGuiManager::Get().GetInspector()->SetGameObject(obj);
			}
			if (ImGui::MenuItem("Box"))
			{
				auto obj = CGameObject::CreateObject().lock();
				obj->AddComponent<Game::CBoxRenderer>();
				// ｺﾝﾎﾟｰﾈﾝﾄの追加後
				ImGuiManager::Get().GetInspector()->SetGameObject(obj);
			}
			if (ImGui::MenuItem("Polygon"))
			{
				auto obj = CGameObject::CreateObject().lock();
				obj->AddComponent<Game::CPolygonRenderer>();
				// ｺﾝﾎﾟｰﾈﾝﾄの追加後
				ImGuiManager::Get().GetInspector()->SetGameObject(obj);
			}
			if (ImGui::MenuItem("Text"))
			{
				auto obj = CGameObject::CreateObject().lock();
				obj->AddComponent<Game::CTextRenderer>();
				// ｺﾝﾎﾟｰﾈﾝﾄの追加後
				ImGuiManager::Get().GetInspector()->SetGameObject(obj);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// 検索更新
	UpdateSearch();

	// ゲームオブジェクト名の表示と子要素の表示
	for (const auto & object : objList)
	{
		// 検索がONになっている時
		if (m_Search.bSearchCriteria)
		{	// 検索条件と一致するか判定し、一致しないならば次へ
			if (!DispCheck(object.get()))
				continue;
		}
#if 1
		// 親要素の確認、親が子を表示するので次へ
		if (object->GetTransform()->GetParent().lock())
			continue;
#endif // 0
		
		// 選択ボタン、ウィンドウ表示
		if (ImGui::Button(object->GetName().c_str()) )
		{
			ImGuiManager::Get().GetInspector()->SetGameObject(object);
			break;
		}

		// ドラッグ設定
		//if (ImGuiManager::Get().GetInspector()->GetSelectObject().lock() == object)
		{
			DragDropSource<CGameObject::PtrWeak>(CHierachy::DESC_SELECT_OBJ, object->GetName(), object);
		}

		// ドラッグ&ドロップされたとき
		if (auto select = DragDropTarget<CGameObject::PtrWeak>(CHierachy::DESC_SELECT_OBJ); select)
			object->GetTransform()->AddChild(select->lock()->GetComponent<CTransform>());

		DispChild(object);

//		// 子の表示
//		if (ImGui::TreeNode(std::string(object->GetName() + "child-").c_str()))
//		{
//			if (object->GetTransform()->GetChild(0).expired())
//			{
//				ImGui::Text(u8"なし");
//			}
//			for (auto cnt = 0; cnt < object->GetTransform()->GetChildCount(); ++cnt)
//			{
//				// nullptr確認
//				if (auto child = object->GetTransform()->GetChild(cnt).lock()->GetOwner(0) ; child.lock())
//				{
//					std::string ownname = child.lock()->GetName();
//					const char* childName = ownname.c_str();
//
//					ImGui::BeginChild(childName, ImVec2(0, 30), false);
//					ImGui::SameLine();
//
//#pragma region CHILD
//					if (select = ImGui::Button(childName); select)
//					{
//						ImGuiManager::Get().GetInspector()->SetGameObject(child);
//					}
//					// ドラッグ設定
//					if (ImGuiManager::Get().GetInspector()->GetSelectObject().lock() == object)
//					{
//						DragDropSource<CGameObject::PtrWeak>(CHierachy::DESC_SELECT_OBJ, object->GetName(), object);
//					}
//
//					// ドラッグ&ドロップされたとき
//					if (auto select = DragDropTarget<CGameObject::PtrWeak>(CHierachy::DESC_SELECT_OBJ); select)
//						object->GetTransform()->AddChild(select->lock()->GetComponent<CTransform>());
//#pragma endregion
//
//					ImGui::EndChild();
//					ImGui::Separator();
//					
//					if (select)break;
//				}
//			}
//			ImGui::TreePop();
//		}
	}
	
	ImGui::End();

	// セーブロード
	if (m_bLoadSaveWindow)
	{
		DispSaveLoadMenu();
	}

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}
void CHierachy::LoadScenePathList()
{
	CFilePath path;
	m_scenePathList = path.GetFileName(m_loadPath);
}
void CHierachy::DispSaveLoadMenu()
{
	bool flg = true;
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_Once);

	ImGui::Begin("Load", &flg, ImGuiWindowFlags_MenuBar);

	//
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("sceneList"))
		{
			for (auto it = m_scenePathList.begin(); it < m_scenePathList.end(); ++it)
			{
				auto name = (*it).c_str();
				if (ImGui::MenuItem(name)) {
					m_loadPath = name;
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	if (ImGui::Button("pathReload")) {
		LoadScenePathList();
	}

	//ImGui::InputText("reloadFile", m_loadPath.data(), 256);
	m_loadPath = InputString(m_loadPath, u8"loadFile");

	if (ImGui::Button("Load"))
	{
		CSceneManager::Get().LoadScene(m_loadPath);
	}
	ImGui::Separator();

	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_Once);

	//ImGui::Begin("Save", &batsu, ImGuiWindowFlags_MenuBar);
	m_savePath = InputString(m_savePath, u8"saveFile");
	if (ImGui::Button(u8"Save"))
	{
		CSceneManager::Get().SaveScene(m_savePath);
	}
	ImGui::End();
}

#pragma region GAME_OBJECT
void CHierachy::DispChild(std::weak_ptr<MySpace::Game::CGameObject> object)
{
	bool select = false;

	ImGui::SameLine();

	// 子の表示
	if (ImGui::TreeNode(std::string(object.lock()->GetName() + "-child-").c_str()))
	{
		if (object.lock()->GetTransform()->GetChildCount() == 0)
		{
			ImGui::Text(u8"なし");
			ImGui::TreePop();
			return;
		}
		
		for (auto cnt = 0; cnt < object.lock()->GetTransform()->GetChildCount(); ++cnt)
		{
			// nullptr確認
			if (auto child = object.lock()->GetTransform()->GetChild(cnt).lock()->GetOwner(0); child.lock())
			{
				std::string ownname = child.lock()->GetName();
				const char* childName = ownname.c_str();

#pragma region CHILD
				if (select = ImGui::Button(childName); select)
				{
					ImGuiManager::Get().GetInspector()->SetGameObject(child);
				}

				// ドラッグ設定
				//if (ImGuiManager::Get().GetInspector()->GetSelectObject().lock() == object.lock())
				{
					DragDropSource<CGameObject::PtrWeak>(CHierachy::DESC_SELECT_OBJ, child.lock()->GetName(), child);
				}

				// ドラッグ&ドロップされたとき
				if (auto selectObj = DragDropTarget<CGameObject::PtrWeak>(CHierachy::DESC_SELECT_OBJ); selectObj)
					child.lock()->GetTransform()->AddChild(selectObj->lock()->GetComponent<CTransform>());
#pragma endregion

				//ImGui::SameLine();
				//--- 子要素を更に表示(再帰)
				//if (auto childObj = child.lock()->GetTransform()->GetChild(0); childObj.lock())
				DispChild(child.lock());

				//ImGui::Separator();

				if (select)
					break;
			}
		}

		ImGui::TreePop();
	}
}
#pragma endregion

#pragma region SEARCH
void CHierachy::UpdateSearch()
{
	// for文変数
	static const int nSearch[static_cast<int>(ESearchTerms::MAX)] =
	{
		static_cast<int>(ESearchTerms::OBJ_NAME),
		static_cast<int>(ESearchTerms::TAG),
		static_cast<int>(ESearchTerms::STATE_ACTIVE),
		static_cast<int>(ESearchTerms::STATE_STOP),
		static_cast<int>(ESearchTerms::STATE_DESTROY),
	};
	static const char* szDisp[static_cast<int>(ESearchTerms::MAX)] =
	{
		"Name","Tag","sActiv","sStop","sDestroy"
	};

	ImGui::Text((m_Search .bSearchCriteria? u8"Search:ON" : u8"Search:OFF"));
	ImGui::SameLine();
	if (ImGui::Button(u8"検索"))
	{
		m_Search.bSearchCriteria ^= true;
	}
	ImGui::SameLine();
	m_Search.inputName = InputString(m_Search.inputName, u8"Search");

	ImGui::Text(u8"Condition");
	for (int cnt = 0; cnt < static_cast<int>(ESearchTerms::MAX); ++cnt)
	{
		ImGui::SameLine();
		ImGui::RadioButton(szDisp[cnt], (int*)&m_Search.eTerms, nSearch[cnt]);
	}
	ImGui::Separator();
}
// 表示する対象か確認
bool CHierachy::DispCheck(CGameObject* obj)
{
	switch (m_Search.eTerms)
	{
	case MySpace::Debug::CHierachy::ESearchTerms::OBJ_NAME:
		// 一部一致
		if (obj->GetName().find(m_Search.inputName) != std::string::npos)
		{
			return true;
		}
		break;
	case MySpace::Debug::CHierachy::ESearchTerms::TAG:
		// tag比較
		return obj->GetTagPtr()->Compare(m_Search.inputName);
	case MySpace::Debug::CHierachy::ESearchTerms::STATE_ACTIVE:
		return obj->GetState() == CGameObject::E_ObjectState::ACTIVE;
	case MySpace::Debug::CHierachy::ESearchTerms::STATE_STOP:
		return obj->GetState() == CGameObject::E_ObjectState::STOP;
	case MySpace::Debug::CHierachy::ESearchTerms::STATE_DESTROY:
		return obj->GetState() == CGameObject::E_ObjectState::DESTROY;
	default:
		break;
	}
	return false;
}
#pragma endregion

#pragma region LIST_SWAP
// list用コンテナ内で挿入入れ替え
// 受け取ったリストを入れ替え返却 else そのまま返却
template<class T>
std::list<T> CHierachy::MovingInList(std::list<T> list, T newT, int index)
{
	// 範囲チェック
	if (index >= list.size())
		return list;

	// 挿入する場所までitを進める
	auto pos = list.begin();
	for (int n = 0; n < index; n++)
	{
		++pos;
	}
	// 探す
	for (auto it = list.begin(); it != list.end(); ++it)
	{
		if (*it == newT)
		{
			// 挿入
			list.insert(pos, *it);
			// 元のオブジェクトは配列から除外
			list.erase(it);
			return list;
		}
	}
	return list;
}

#pragma endregion


#endif // !BUILD_MODE
