//==========================================================
// [pauseMenu.h]
//----------------------------------------------------------
// 作成:2023/06/08
// 
//==========================================================

#include <GameSystem/DarkGame/pauseMenu.h>
#include <DebugSystem/imGuiPackage.h>
#include <DebugSystem/hierarchy.h>

using namespace DarkGame;

//==========================================================
// コンストラクタ
//==========================================================
CPauseMenu::CPauseMenu()
{

}

//==========================================================
// コンストラクタ
//==========================================================
CPauseMenu::CPauseMenu(CGameObject::Ptr ptr)
	:CComponent(ptr),m_nCursor(0), m_eMode(EMenuMode::None)
{
	
}

//==========================================================
// デストラクタ
//==========================================================
CPauseMenu::~CPauseMenu()
{

}

//==========================================================
// 生成時処理
//==========================================================
void CPauseMenu::Awake()
{

	CTag::CreateTag(TAG);
	GetOwner()->SetObjTag(TAG);

	UIEnable(false);
}

//==========================================================
// 初期化
//==========================================================
void CPauseMenu::Init()
{

}

//==========================================================
// 更新
//==========================================================
void CPauseMenu::Update()
{
	if (m_eMode != EMenuMode::Select)
		return;

	if (m_aPolygon.size() != (int)ESelect::Max)
		return;

	//--- ラムダ式
	// 選択中のUIを強調
	auto uiHighLight = [=](int id, float alpha)->void{
		const auto ui = m_aPolygon[m_nCursor % (int)ESelect::Max];
		auto color = ui.lock()->GetColor();
		color.a = alpha;
		ui.lock()->SetColor(color);		
	};

	//--- 入力処理
	if (1)
	{
		--m_nCursor;
		uiHighLight((m_nCursor) % (int)ESelect::Max, 1.0f);
		uiHighLight((m_nCursor ^ 1) % (int)ESelect::Max, 0.5f);
	}
	else if (2)
	{
		++m_nCursor;
		uiHighLight((m_nCursor) % (int)ESelect::Max, 1.0f);
		uiHighLight((m_nCursor ^ 1) % (int)ESelect::Max, 0.5f);
	}
	else if (3)
	{
		m_eMode = EMenuMode::Fixed;
	}

}

//==========================================================
// 選択状態取得
//==========================================================
int CPauseMenu::Select()
{
	if (m_eMode != EMenuMode::Fixed)
		return -1;

	// 状態を初期化
	m_eMode = EMenuMode::None;
	return m_nCursor % (int)ESelect::Max;
}

//==========================================================
// UI有効切替
//==========================================================
void CPauseMenu::UIEnable(const bool& enable)
{
	// 有効化時は選択状態にも移行
	if(enable)
		m_eMode = EMenuMode::Select;

	for (const auto & ui : m_aPolygon)
	{
		ui.lock()->SetActive(enable);
	}
}


#if BUILD_MODE

// ドラッグアンドドロップで登録
// クリア
void CPauseMenu::ImGuiDebug()
{
	using namespace MySpace;
	
	Debug::SetTextAndAligned("UI_Object");
	ImGui::Text("%d", m_aPolygon.size());


	Debug::SetTextAndAligned("");
	if (ImGui::Button("Clear"))
	{
		m_aPolygon.clear();
	}

	Debug::SetTextAndAligned("Add UI");
	ImGui::Text("##Add UI");
	
	// drag&drop設定
	Debug::DragDropSource<CGameObject::PtrWeak>(Debug::CHierachy::DESC_SELECT_OBJ, this->GetName().c_str(), this->GetOwner()->GetPtr());

	// drag&dropされた時
	if (auto select = Debug::DragDropTarget<CGameObject::PtrWeak>(Debug::CHierachy::DESC_SELECT_OBJ); select)
	{
		auto polygon = select->lock()->GetComponent<CPolygonRenderer>();
		// PolygonRenderer以外はNG
		if(polygon.lock())
			m_aPolygon.push_back(polygon);
	}

}

#endif // BUILD_MODE
