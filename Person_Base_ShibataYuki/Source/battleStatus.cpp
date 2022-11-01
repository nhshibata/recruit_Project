//=========================================================
// [battleStatus.cpp]
// 作成:2022/09/21
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <battleStatus.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>

using namespace Spell;

void CBattleStatus::Awake()
{
	// TODO: 2Dか、3Dか、エフェクトで表示するか検討
	//CGameObject::Ptr obj;
	//CPolygonRenderer* render;
	//// 描画追加
	//for (int cnt = 0; cnt < 10; ++cnt)
	//{
	//	obj = CGameObject::CreateObject().lock();
	//	render = obj->AddComponent<CPolygonRenderer>().get();
	//	render->GetRectTransform()->SetPos();
	//}
	//m_pLifeRender;

	// 標準
	this->Life.accessor = 10;
	this->Atk.accessor = 1;
	this->Damage.accessor = 0;
}
void CBattleStatus::Init()
{
}
void CBattleStatus::Update()
{
	if (this->Damage.accessor == 0)	// ダメージがないなら抜ける
		return;
	if (CFps::Get().GetTime() % 4 == 0)	// 間隔を空ける
		return;

	int add = 0;
	add = (0 - this->Damage.accessor) / 10;
	this->Life.accessor += add;
}
void CBattleStatus::FixedUpdate()
{

}
