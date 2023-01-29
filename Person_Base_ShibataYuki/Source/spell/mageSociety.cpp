//=========================================================
// [mageSociety.h] 
// 作成:2022/09/29
//---------------------------------------------------------
// キャラクター管理
//=========================================================

//--- インクルード部
#include <mageSociety.h>
#include <mage.h>

using namespace Spell;

void CMageSociety::Awake()
{

}
void CMageSociety::Init()
{
	auto objs = CGameObject::FindGameObjectsWithTag(CTag("Mage"));
	for (auto & obj : objs)
	{
		if (auto mage = obj.lock()->GetComponent<CMage>(); mage.lock())
		{
			m_spMages.push_back(mage);
		}
	}
}
void CMageSociety::Update()
{

}
void CMageSociety::FixedUpdate()
{
	for (auto it = m_spMages.begin(); it != m_spMages.end();)
	{
		if ((*it).lock()->Status.accessor->IsLife())
		{
			// 消去する
			// TODO: 消去する際の演出はどうするか?
			it = m_spMages.erase(it);
		}

		++it;
	}
}