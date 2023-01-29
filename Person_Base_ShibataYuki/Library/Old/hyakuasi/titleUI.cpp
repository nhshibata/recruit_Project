//=========================================================
// [titleUI.cpp] 
// çÏê¨:2022/08/18
//---------------------------------------------------------
//=========================================================

#include <titleUI.h>
#include <UIAnime.h>
#include <GameSystem/Component/Renderer/textRenderer.h>

//
CTitleUI::CTitleUI(std::shared_ptr<CGameObject> owner)
	:CComponent(owner)
{

}
void CTitleUI::Awake()
{
	auto scrSize = CScreen::GetSize();

	GetOwner()->SetName("TitleSceneUI");

	// BG
	auto obj = CGameObject::CreateObject().lock();
	auto polygon = obj->AddComponent<CPolygonRenderer>();
	polygon->SetImageName(TEXTURE_NAME(title_BG.png));
	polygon->GetRectTransform()->SetSize(scrSize);
	obj->SetLayer(CLayer::E_Layer::BG);
	Transform()->AddChild(obj);

	// ÉçÉS
	obj = CGameObject::CreateObject().lock();
	obj->SetLayer(CLayer::E_Layer::UI);
	polygon = obj->AddComponent<CPolygonRenderer>();
	polygon->SetImageName(TEXTURE_NAME(title_Logo.png));
	polygon->GetRectTransform()->SetPos(Vector2(0.0f,
		polygon->GetRectTransform()->GetPos().y + scrSize.y / 3));
	polygon->GetRectTransform()->SetSize(scrSize.x / 3, scrSize.y / 3);
	Transform()->AddChild(obj);

	// É{É^Éì
	obj = CGameObject::CreateObject().lock();
	obj->SetLayer(CLayer::E_Layer::UI);
	polygon = obj->AddComponent<CPolygonRenderer>();
	polygon->SetImageName(TEXTURE_NAME(title_Button.png));
	polygon->GetRectTransform()->SetPos(Vector2(0.0f,
		polygon->GetRectTransform()->GetPos().y - scrSize.y / 3));
	polygon->GetRectTransform()->SetSize(scrSize.x / 5, scrSize.y / 5);
	Transform()->AddChild(obj);
	
	// mukade
	obj = CGameObject::CreateObject().lock();
	obj->AddComponent<CUIAnimeCurve>();

	// mukade
	obj = CGameObject::CreateObject().lock();
	obj->AddComponent<CUIAnimeCurve>();
}
void CTitleUI::Init()
{

}
void CTitleUI::Update()
{
	
}
