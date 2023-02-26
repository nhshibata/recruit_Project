//=========================================================
// [hirarchy.cpp] 
// 作成:2022/07/10
// 更新:2022/11/12 Drag&Dropで親子関係
//---------------------------------------------------------
// GameObjectを表示、操作する
//=========================================================

//--- インクルード部
#include <DebugSystem/hierarchy.h>
#if BUILD_MODE

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_win32.h>
#include <ImGui/imgui_impl_dx11.h>
#include <DebugSystem/imguiManager.h>
#include <DebugSystem/inspector.h>
#include <DebugSystem/imGuiPackage.h>

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

//==========================================================
// コンストラクタ
//==========================================================
CHierachy::CHierachy()
	:m_bLoadSaveWindow(false)
{
	m_strSavePath.clear();
	m_strLoadPath = FORDER_DIR(Data/scene);
	LoadScenePathList();
}

//==========================================================
// デストラクタ
//==========================================================
CHierachy::~CHierachy()
{
}

//==========================================================
// 更新
//==========================================================
void CHierachy::Update(ImGuiManager* manager)
{
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(250, 400), ImGuiCond_Once);
	bool flg = true;
	ImGui::Begin(u8"Hierarchy", &flg, ImGuiWindowFlags_MenuBar);

	// シーンが存在していなければ処理しない
	if (!CSceneManager::Get()->GetActiveScene())
		return;

	manager->HoverStateSet();

	//--- メニューバー
	if (ImGui::BeginMenuBar())
	{
		//--- シーン
		if (ImGui::BeginMenu("Scene File"))
		{
			if (ImGui::MenuItem("New empty Scene"))
			{
				CSceneManager::Get()->CreateNewScene<CScene>();
				//auto newScene = CSceneManager::Get()->CreateNewScene<CScene>();
				//CSceneManager::Get()->SetActiveScene(CSceneManager::Get()->GetSceneByName(newScene->GetSceneName()));
			}
			if (ImGui::MenuItem("New Scene"))
			{
				auto newScene = CSceneManager::Get()->CreateNewScene<CScene>();
				newScene->CreateEmptyScene();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Load/Save"))
			{
				m_bLoadSaveWindow = !m_bLoadSaveWindow;
			}
			ImGui::EndMenu();
		}

		//--- オブジェクト生成
		if (ImGui::BeginMenu("New GameObject"))
		{
			if (ImGui::MenuItem("Empty"))
				manager->GetInspector()->SetSelectGameObject(CreateObject(0));
			if (ImGui::MenuItem("Model"))
				manager->GetInspector()->SetSelectGameObject(CreateObject(1));
			if (ImGui::MenuItem("StaticModel"))
				manager->GetInspector()->SetSelectGameObject(CreateObject(2));
			if (ImGui::MenuItem("Billboard"))
				manager->GetInspector()->SetSelectGameObject(CreateObject(3));
			if (ImGui::MenuItem("Sphere"))
				manager->GetInspector()->SetSelectGameObject(CreateObject(4));
			if (ImGui::MenuItem("Box"))
				manager->GetInspector()->SetSelectGameObject(CreateObject(5));
			if (ImGui::MenuItem("Polygon"))
				manager->GetInspector()->SetSelectGameObject(CreateObject(6));			
			if (ImGui::MenuItem("Text"))
				manager->GetInspector()->SetSelectGameObject(CreateObject(7));
			if (ImGui::MenuItem("Objects"))
			{
				m_CreateValue.bDisp ^= true;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// 検索更新
	DispSearch();

	// オブジェクトウィンドウ
	if(m_CreateValue.bDisp)
		CreateObjectsWindow();

	//--- GameObject表示
	auto objList = CSceneManager::Get()->GetActiveScene()->GetObjManager()->GetList();

	// ゲームオブジェクト名の表示と子要素の表示
	for (const auto & object : objList)
	{
		// 検索がONになっている時
		if (m_Search.bSearchCriteria)
		{	// 検索条件と一致するか判定し、一致しないならば次へ
			if (!DispCheck(object.get()))
				continue;
		}

		// 親要素の確認、親が子を表示するので次へ
		if (object->GetTransform()->GetParent().lock())
			continue;
		
		// 選択ボタン、ウィンドウ表示
		if (ImGui::Button(object->GetName().c_str()))
		{
			manager->GetInspector()->SetSelectGameObject(object);
			break;
		}

		// ドラッグ設定
		//if (ImGuiManager::Get()->GetInspector()->GetSelectObject().lock() == object)
		{
			DragDropSource<CGameObject::PtrWeak>(CHierachy::DESC_SELECT_OBJ, object->GetName(), object);
		}

		// ドラッグ&ドロップされたとき
		if (auto select = DragDropTarget<CGameObject::PtrWeak>(CHierachy::DESC_SELECT_OBJ); select)
			object->GetTransform()->AddChild(select->lock()->GetComponent<CTransform>());

		//--- 子要素の表示
		DispChild(manager, object);
	}
	
	ImGui::End();

	//--- セーブロード
	if(m_bLoadSaveWindow)
		DispSaveLoadMenu();
	

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

//==========================================================
// シーンファイル
//==========================================================
void CHierachy::LoadScenePathList()
{
	CFilePath path;
	m_aScenePathList = path.GetFileName(m_strLoadPath);
}

//==========================================================
// Scene保存読み込みメニュー表示
//==========================================================
void CHierachy::DispSaveLoadMenu()
{
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_Once);

	if (ImGui::Begin("Load", &m_bLoadSaveWindow, ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("SceneList"))
			{
				for (auto it = m_aScenePathList.begin(); it < m_aScenePathList.end(); ++it)
				{
					auto name = (*it).c_str();
					if (ImGui::MenuItem(name))
					{
						m_strLoadPath = name;
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		if (ImGui::Button("Scene Path Reload"))
		{
			LoadScenePathList();
		}

		m_strLoadPath = InputString(m_strLoadPath, "LoadFile");

		if (ImGui::Button("Load"))
		{
			if (CSceneManager::Get()->LoadScene(m_strLoadPath))
				m_bLoadSaveWindow = false;
		}
		ImGui::Separator();


		ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_Once);

		m_strSavePath = InputString(m_strSavePath, "SaveFile");
		if (ImGui::Button("Save"))
		{
			CSceneManager::Get()->SaveScene(m_strSavePath);
		}
		ImGui::Separator();
		if (ImGui::Button("SaveScene OverWrite"))
		{
			CSceneManager::Get()->SaveScene(m_strSavePath);
		}
		if (ImGui::Button("LoadScene OverWrite"))
		{
			CSceneManager::Get()->SaveScene(m_strLoadPath);
		}

		ImGui::End();
	}
}

#pragma region GAME_OBJECT

//==========================================================
// 子オブジェクト表示
//==========================================================
void CHierachy::DispChild(ImGuiManager* manager, std::weak_ptr<MySpace::Game::CGameObject> object)
{
	bool select = false;

	ImGui::SameLine();

	// 子の表示
	if (ImGui::TreeNode(std::string(object.lock()->GetName() + "-child-").c_str()))
	{
		if (object.lock()->GetTransform()->GetChildCount() == 0)
		{
			ImGui::Text("none");
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
					manager->GetInspector()->SetSelectGameObject(child);
				}

				// ドラッグ設定
				//if (ImGuiManager::Get()->GetInspector()->GetSelectObject().lock() == object.lock())
				{
					DragDropSource<CGameObject::PtrWeak>(CHierachy::DESC_SELECT_OBJ, child.lock()->GetName(), child);
				}

				// ドラッグ&ドロップされたとき
				if (auto selectObj = DragDropTarget<CGameObject::PtrWeak>(CHierachy::DESC_SELECT_OBJ); selectObj)
					child.lock()->GetTransform()->AddChild(selectObj->lock()->GetComponent<CTransform>());
#pragma endregion

				//--- 子要素を更に表示(再帰)
				//if (auto childObj = child.lock()->GetTransform()->GetChild(0); childObj.lock())
				DispChild(manager, child.lock());

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

//==========================================================
// 表示オブジェクト切替
//==========================================================
void CHierachy::DispSearch()
{
	// for文変数
	static const int nSearch[static_cast<int>(ESearchTerms::MAX)] =
	{
		static_cast<int>(ESearchTerms::OBJ_NAME),
		static_cast<int>(ESearchTerms::TAG),
		static_cast<int>(ESearchTerms::COMPONENT),
		static_cast<int>(ESearchTerms::STATE_ACTIVE),
		static_cast<int>(ESearchTerms::STATE_STOP),
		static_cast<int>(ESearchTerms::STATE_DESTROY),
	};

	static const char* szDisp[static_cast<int>(ESearchTerms::MAX)] =
	{
		"Name","Tag","Component","sActive","sStop","sDestroy"
	};

	ImGui::Text((m_Search .bSearchCriteria? u8"Search:ON" : u8"Search:OFF"));
	ImGui::SameLine();
	if (ImGui::Button(u8"検索"))
	{
		m_Search.bSearchCriteria ^= true;
	}
	ImGui::SameLine();

	//--- 入力
	m_Search.inputName = InputString(m_Search.inputName, u8"Search");

	ImGui::Text(u8"Condition");
	for (int cnt = 0; cnt < static_cast<int>(ESearchTerms::MAX); ++cnt)
	{
		ImGui::SameLine();
		ImGui::RadioButton(szDisp[cnt], (int*)&m_Search.eTerms, nSearch[cnt]);
	}
	ImGui::Separator();
}

//==========================================================
// 表示する対象確認
//==========================================================
bool CHierachy::DispCheck(CGameObject* obj)
{
	switch (m_Search.eTerms)
	{
	case MySpace::Debug::CHierachy::ESearchTerms::OBJ_NAME:		// 文字列一部一致でも可
		if (obj->GetName().find(m_Search.inputName) != std::string::npos)
			return true;
		break;
	case MySpace::Debug::CHierachy::ESearchTerms::TAG:			// tag比較
		return obj->GetTagPtr()->Compare(m_Search.inputName);

	case MySpace::Debug::CHierachy::ESearchTerms::COMPONENT:
	{
		auto comList = obj->GetComponentList();					// 文字列一部一致でも可
		for (const auto & com : comList)
		{
			if (com->GetName().find(m_Search.inputName) != std::string::npos)
				return true;
		}
		break;
	}
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


//==========================================================
// list用コンテナ内で挿入入れ替え
// 受け取ったリストを入れ替え返却 else そのまま返却
//==========================================================
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

//==========================================================
// オブジェクト一括生成
//==========================================================
void CHierachy::CreateObjectsWindow()
{
	static const char* aSelectType[] ={
		"empty", 
		"Model",
		"StaticModel",
		"Billboard",
		"Sphere",
		"Box",
		"Polygon",
		"Text"
	};
	ImGui::Begin("Objects Create", &m_CreateValue.bDisp);

	// type選択
	for (auto cnt = 0; cnt < _countof(aSelectType); ++cnt)
	{
		if (ImGui::Selectable(aSelectType[cnt], m_CreateValue.nObjType))
		{
			m_CreateValue.nObjType = cnt;
			break;
		}
	}

	ImGui::DragInt("Grid", &m_CreateValue.nGrid);
	ImGui::DragFloat("Margin", &m_CreateValue.fMargin);
	ImGui::DragFloat3("Center", (float*)&m_CreateValue.vCenter);
	// 実行
	if (ImGui::Button("Create"))
	{
		for (int grid = 0; grid < m_CreateValue.nGrid * m_CreateValue.nGrid; ++grid)
		{
			float col = float(grid % m_CreateValue.nGrid);
			float row = float(grid / m_CreateValue.nGrid);
			auto pObj = CreateObject(m_CreateValue.nObjType);
			auto newPos = m_CreateValue.vCenter;
			newPos.x = (m_CreateValue.vCenter.x - (m_CreateValue.nGrid - 1 * m_CreateValue.fMargin * 0.5f)) + col * m_CreateValue.fMargin;
			newPos.z = (m_CreateValue.vCenter.z - (m_CreateValue.nGrid - 1 * m_CreateValue.fMargin * 0.5f)) + row * m_CreateValue.fMargin;
			pObj->GetTransform()->SetPos(newPos);
		}
	}

	ImGui::End();

}

//==========================================================
// オブジェクト生成
//==========================================================
CGameObject::Ptr CHierachy::CreateObject(int No, std::shared_ptr<MySpace::Game::CGameObject> copy)
{
	CGameObject::Ptr obj;
	if (copy)
		obj = CGameObject::CopyObject(copy).lock();
	else
		obj = CGameObject::CreateObject().lock();

	switch (No)
	{
	case 0:
		break;
	case 1:
		obj->AddComponent<CModelRenderer>();
		break;
	case 2:
	{
		auto render = obj->AddComponent<CModelRenderer>();
		render->SetStatic(CMeshRenderer::EStaticMode::STATIC);
		break;
	}
	case 3:
		obj->AddComponent<Game::CBillboardRenderer>();
		break;
	case 4:
		obj->AddComponent<Game::CSphereRenderer>();
		break;
	case 5:
		obj->AddComponent<Game::CBoxRenderer>();
		break;
	case 6:
		obj->AddComponent<Game::CPolygonRenderer>();
		break;
	case 7:
		obj->AddComponent<Game::CTextRenderer>();
		break;
	default:
		break;
	};

	return obj;
}

#endif // !BUILD_MODE
