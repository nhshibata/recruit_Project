//==========================================================
// [pauseMenu.h]
//----------------------------------------------------------
// �쐬:2023/06/08
// 
//==========================================================

#include <GameSystem/DarkGame/pauseMenu.h>
#include <DebugSystem/imGuiPackage.h>
#include <DebugSystem/hierarchy.h>

using namespace DarkGame;

//==========================================================
// �R���X�g���N�^
//==========================================================
CPauseMenu::CPauseMenu()
{

}

//==========================================================
// �R���X�g���N�^
//==========================================================
CPauseMenu::CPauseMenu(CGameObject::Ptr ptr)
	:CComponent(ptr),m_nCursor(0), m_eMode(EMenuMode::None)
{
	
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CPauseMenu::~CPauseMenu()
{

}

//==========================================================
// ����������
//==========================================================
void CPauseMenu::Awake()
{

	CTag::CreateTag(TAG);
	GetOwner()->SetObjTag(TAG);

	UIEnable(false);
}

//==========================================================
// ������
//==========================================================
void CPauseMenu::Init()
{

}

//==========================================================
// �X�V
//==========================================================
void CPauseMenu::Update()
{
	if (m_eMode != EMenuMode::Select)
		return;

	if (m_aPolygon.size() != (int)ESelect::Max)
		return;

	//--- �����_��
	// �I�𒆂�UI������
	auto uiHighLight = [=](int id, float alpha)->void{
		const auto ui = m_aPolygon[m_nCursor % (int)ESelect::Max];
		auto color = ui.lock()->GetColor();
		color.a = alpha;
		ui.lock()->SetColor(color);		
	};

	//--- ���͏���
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
// �I����Ԏ擾
//==========================================================
int CPauseMenu::Select()
{
	if (m_eMode != EMenuMode::Fixed)
		return -1;

	// ��Ԃ�������
	m_eMode = EMenuMode::None;
	return m_nCursor % (int)ESelect::Max;
}

//==========================================================
// UI�L���ؑ�
//==========================================================
void CPauseMenu::UIEnable(const bool& enable)
{
	// �L�������͑I����Ԃɂ��ڍs
	if(enable)
		m_eMode = EMenuMode::Select;

	for (const auto & ui : m_aPolygon)
	{
		ui.lock()->SetActive(enable);
	}
}


#if BUILD_MODE

// �h���b�O�A���h�h���b�v�œo�^
// �N���A
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
	
	// drag&drop�ݒ�
	Debug::DragDropSource<CGameObject::PtrWeak>(Debug::CHierachy::DESC_SELECT_OBJ, this->GetName().c_str(), this->GetOwner()->GetPtr());

	// drag&drop���ꂽ��
	if (auto select = Debug::DragDropTarget<CGameObject::PtrWeak>(Debug::CHierachy::DESC_SELECT_OBJ); select)
	{
		auto polygon = select->lock()->GetComponent<CPolygonRenderer>();
		// PolygonRenderer�ȊO��NG
		if(polygon.lock())
			m_aPolygon.push_back(polygon);
	}

}

#endif // BUILD_MODE
