//=========================================================
// [mage.cpp]
// 作成:2022/09/29
//---------------------------------------------------------
// キャラクター
//=========================================================

//--- インクルード部
#include <mage.h>

using namespace Spell;

void CMage::Awake()
{
	// タグの作成
	GetOwner()->GetTagPtr()->CreateTag(CMage::TAG_NAME);
	
	// 必要なｺﾝﾎﾟｰﾈﾝﾄの追加
	m_pCommand = AddComponent<CCommandSpell>(0);
	Status.accessor = AddComponent<CBattleStatus>(0);

}
void CMage::Init()
{
	// 
}
void CMage::Update()
{
	// 取得
	if (STSpell* pSpell = m_pCommand->Present(); pSpell)
	{
		STSpell spell = *pSpell;

		m_pCommand->DestroyPresent();
	}

	// 取得した情報をUIへ渡す

}