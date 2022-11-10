//=========================================================
// [inspector.cpp] 
// 作成:2022/07/10
// 更新:2022/09/11 ﾄﾗﾝｽﾌｫｰﾑの値をキー入力で変更実装
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <DebugSystem/inspector.h>

#if BUILD_MODE
#include <DebugSystem/imguiManager.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_win32.h>
#include <ImGui/imgui_impl_dx11.h>

#include <GameSystem/Scene/scene.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/gameObjectManager.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Component/Transform/rectTransform.h>
#include <GameSystem/Factory/componentFactory.h>
#include <CoreSystem/Input/input.h>

using namespace MySpace::System;
using namespace MySpace::Game;
using namespace MySpace::SceneManager;
using namespace MySpace::Debug;

CInspector::CInspector()
	:m_isComponent(false)
{
}
CInspector::~CInspector()
{

}
void CInspector::Init()
{

}
void CInspector::Uninit()
{

}
void CInspector::Update()
{
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(800, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400, 600), ImGuiCond_Once);
	
	ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar);

	if (!m_spViewObj.lock())
	{
		ImGui::End();

		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		return;
	}

	// 選択中objデバッグ表示
	DebugObject();
	
	// 選択中objの移動
	//MoveObject();
	
	if (ImGui::IsWindowHovered())
	{	
		ImGuiManager::Get().UpHover(ImGuiManager::EIsHovered::HOVERED_WINDOW);
		ImGui::Text("aiueu");
	}

	if (ImGui::BeginMenuBar())
	{
		// オブジェクト生成
		if (ImGui::BeginMenu("Option"))
		{
			if (ImGui::MenuItem("AddComponent"))
			{
				m_isComponent = !m_isComponent;
			}

			if (ImGui::MenuItem("Copy"))
			{
				Copy();
			}

			if (ImGui::MenuItem("Delete"))
				DeleteObject();

			if (ImGui::MenuItem("Close"))
				m_spViewObj.reset();

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (m_isComponent)
		AddComponentWindow();

	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}
void CInspector::Draw()
{
}
void CInspector::DeleteObject()
{
	CGameObject::Destroy(m_spViewObj.lock());
	m_spViewObj.reset();
}
void CInspector::SetGameObject(std::weak_ptr<CGameObject> obj)
{
	m_spViewObj = obj;
	
	// 移動用に取得（なくても構わない
	m_TransformController.spRect = m_spViewObj.lock()->GetComponent<CRectTransform>();
	m_isDrawInfo.clear();
	int n = static_cast<int>(obj.lock()->GetComponentList().size()) + 1;
	for (int cnt = 0; cnt < n; ++cnt)
	{
		m_isDrawInfo.push_back(false);
	}
}
void CInspector::DeleteInformation()
{
	for (unsigned int i = 1; i < m_isDrawInfo.size(); i++)
	{
		if (m_isDrawInfo[i] == true)
		{
			m_isDrawInfo.erase(m_isDrawInfo.begin() + i);
			m_isDeleted = true;
			return;
		}
	}
}
void CInspector::Copy()
{
	if (!m_spViewObj.lock()) return;

	m_spViewObj = CGameObject::CreateObject(m_spViewObj.lock().get());
	//CGameObject* obj = new CGameObject(*m_object->obj.lock());
	//std::shared_ptr<CGameObject> spobj = std::shared_ptr<CGameObject>(obj);
	//SceneManager::CSceneManager::Get().GetActiveScene()->GetObjManager()->SetGameObject(spobj);
}
void CInspector::DebugObject()
{
	if (!m_spViewObj.lock()) return;

	int layer;
	m_isDeleted = false;
	// 表示オブジェクトのステータス表示
	m_spViewObj.lock()->ImGuiDebug();
	m_spViewObj.lock()->GetTransform()->ImGuiDebug();

	// レイヤー
	layer = *m_spViewObj.lock()->GetLayerPtr()->GetLayer();
	ImGui::InputInt("layer", &layer);
	/*if (layer >= 10)
	{
		layer = 10;
	}*/
	m_spViewObj.lock()->GetLayerPtr()->SetLayer(layer);

	// ｺﾝﾎﾟｰﾈﾝﾄ
	int cnt = 0;
	auto components = m_spViewObj.lock()->GetComponentList();
	if (m_isDrawInfo.size() != components.size())
	{
		m_isDrawInfo.resize(components.size());
	}
	for (auto com : components)
	{
		std::string name = typeid(*com).name();
		if (ImGui::Button(name.substr(6).c_str()))
		{
			m_isDrawInfo[cnt] = !m_isDrawInfo[cnt];
		}
		cnt++;
	}

	cnt = 0;
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(1000, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Once);
	for (auto com : components)
	{
		if (m_isDrawInfo[cnt])
		{
			std::string name = com->GetName();
			
			//ImGui::Begin(name.substr(6).c_str(), nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar);

			// ｺﾝﾎﾟｰﾈﾝﾄのデバッグ表示
			com->ImGuiDebug();

			if (ImGui::Button(u8"Delete"))
			{
				m_spViewObj.lock()->RemoveComponent(com);
			}

			//ImGui::End();
			if (m_isDeleted) break;;
		}
		cnt++;
	}
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}
void CInspector::AddComponentWindow()
{
	bool batsu = true;
	ImGui::SetNextWindowPos(ImVec2(1000, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Once);
	ImGui::Begin(u8"AddComponent", &batsu, ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar);
	std::vector<std::string> componentName = CComponentFactory::GetNameList();
	
	// オブジェクトにコンポーネントを追加
	for (std::string str : componentName)
	{
		if (ImGui::Button(str.c_str()))
		{
			// コンポーネントを保存しているｸﾗｽに追加してもらう
			CComponentFactory::ObjSetComponent(*m_spViewObj.lock(), str);
			
			// 最後尾に追加されているコンポーネントを取得
			auto com = m_spViewObj.lock()->GetComponentList().back();
			// 初期化処理を行う。通常、静的であれば必要ないが、メインループ中の追加のため、ここで行わなければならない
			com->Init();

			m_isDrawInfo.push_back(false);
			m_isComponent = false;
			break;
		}
	}
	ImGui::End();
}
void CInspector::MoveObject()
{
	if (!ImGuiManager::Get().GetPause())
		return;
	if (!m_spViewObj.lock()) return;

	// TODO:rectかどうかで処理を変える(少し長いし無駄ありそう)
	float* value;
	// nullでなければ0を、nullならxyz分のサイズ
	int nSize = m_TransformController.spRect.lock() != nullptr ? 0 : 3;
	if (nSize)
		value = new float[nSize];

	// 操作させる変数を変更
	if (Input::Keyboad::IsTrigger(VK_X))
	{
		int newType = static_cast<int>(m_TransformController.eType);
		newType = (newType + 1 >= static_cast<int>(EObjMoveType::MAX) ? static_cast<int>(EObjMoveType::POSITION) : newType + 1);
		m_TransformController.eType = static_cast<EObjMoveType>(newType);
	}

	switch (m_TransformController.eType)
	{
	case MySpace::Debug::CInspector::EObjMoveType::POSITION:
	{
		if (!nSize)
		{
			nSize = 2;
			value = new float[nSize];
			Value(value, &m_TransformController.spRect.lock()->GetPos());
		}
		else
			Value(value, &m_spViewObj.lock()->GetTransform()->GetPos());
		break;
	}
	case MySpace::Debug::CInspector::EObjMoveType::ROTATE:
	{
		if (!nSize)
		{
			nSize = 1;
			value = new float[nSize];
			Value(value, m_TransformController.spRect.lock()->GetAngle());
		}
		else
			Value(value, &m_spViewObj.lock()->GetTransform()->GetRot());
		break; 
	}
	case MySpace::Debug::CInspector::EObjMoveType::SCALE:
	{
		if (!nSize)
		{
			nSize = 2;
			value = new float[nSize];
			Value(value, &m_TransformController.spRect.lock()->GetSize());
		}
		else
			Value(value, &m_spViewObj.lock()->GetTransform()->GetScale());
		break;
	}
	default:
		break;
	}

	// WASDQE(xyz)
	if (Input::Keyboad::IsTrigger(VK_A))
		value[0] -= m_TransformController.fMoveValue;
	if (Input::Keyboad::IsTrigger(VK_D))
		value[0] += m_TransformController.fMoveValue;
	if (nSize >= 2)
	{
		if (Input::Keyboad::IsTrigger(VK_Q))
			value[1] -= m_TransformController.fMoveValue;
		if (Input::Keyboad::IsTrigger(VK_E))
			value[1] += m_TransformController.fMoveValue;
	}
	if (nSize == 3)
	{
		if (Input::Keyboad::IsTrigger(VK_W))
			value[2] += m_TransformController.fMoveValue;
		if (Input::Keyboad::IsTrigger(VK_S))
			value[2] -= m_TransformController.fMoveValue;
	}
	// 一フレームで操作する値の調整
	m_TransformController.fMoveValue += Input::Mouse::GetHwheel();
	if (m_TransformController.fMoveValue > 5.0f)
		m_TransformController.fMoveValue = 5.0f;
	if (m_TransformController.fMoveValue < 0.5f)
		m_TransformController.fMoveValue = 0.5f;

	switch (m_TransformController.eType)
	{
	case MySpace::Debug::CInspector::EObjMoveType::POSITION:
		if (nSize)
			m_TransformController.spRect.lock()->SetPos({value[0],value[1]});
		else
			m_spViewObj.lock()->GetTransform()->SetPos({ value[0],value[1],value[2] });
		break;
	case MySpace::Debug::CInspector::EObjMoveType::ROTATE:
		if (nSize)
			m_TransformController.spRect.lock()->SetAngle({ value[0] });
		else
			m_spViewObj.lock()->GetTransform()->SetRot({ value[0],value[1],value[2] });
		break;
	case MySpace::Debug::CInspector::EObjMoveType::SCALE:
		if (nSize)
			m_TransformController.spRect.lock()->SetSize({ value[0],value[1] });
		else
			m_spViewObj.lock()->GetTransform()->SetScale({ value[0],value[1],value[2] });
		break;
	default:
		break;
	}

	delete[] value;
}
void CInspector::Value(float* value, Vector3* vec)
{
	value[0] = vec->x; value[1] = vec->y; value[2] = vec->z;
}
void CInspector::Value(float* value, Vector2* vec)
{
	value[0] = vec->x; value[1] = vec->y;
}
void CInspector::Value(float* value, float vec)
{
	value[0] = vec;
}

#endif // !BUILD_MODE
