//=========================================================
// [itemBase.cpp]
// 作成:2022/08/16
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <itemBase.h>
#include <GameSystem/Component/Renderer/billboardRenderer.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Component/Collision/sphereCollision.h>
#include <CoreSystem/Sound/Sound.h>

CItemBase::CItemBase(std::shared_ptr<CGameObject> owner)
	:CComponent(owner)
{
}
void CItemBase::Awake()
{
	GetOwner()->GetTagPtr()->CreateTag("Item");
	
	auto col = GetOwner()->AddComponent<CSphereCollision>();
	col->SetTrigger(true);
	col->SetRadius(10);
	GetOwner()->SetLayer(CLayer::E_Layer::MODEL);

	// 画像
	auto render = GetOwner()->AddComponent<CBillboardRenderer>();
	render->SetSprite(TEXTURE_NAME(item.png));
	Transform()->SetScale({10,10,10});
}
void CItemBase::OnTriggerStay(CGameObject* other)
{
	// 1は一致。1超えは含まれている
	if (other->GetTagPtr()->Compare("Player") == 1)
	{
		--m_nUseCnt;
		CSound::Play(eSE::SE_GET);
		CSound::SetVolume(eSE::SE_GET, 1.5f);

		if (m_nUseCnt <= 0)
		{
			CSound::Play(eSE::SE_GET);
			CSound::SetVolume(eSE::SE_GET, 1.5f);
			GetOwner()->SetState(CGameObject::E_ObjectState::TAKEOVER);
			m_pManager.lock()->SetStandby(BaseToDerived<CItemBase>());
		}
	}
#ifdef _DEBUG
	else if (other->GetTagPtr()->Compare("Item"))
	{

	}
	else
	{
		auto ane = other->GetName();
		auto an = GetOwner()->GetName();
	}
#endif // _DEBUG
}
void CLifeItem::Awake()
{
	CItemBase::Awake();
	// 回復アイテム
	GetOwner()->GetTagPtr()->CreateTag("LifeItem");
	GetComponent<CBillboardRenderer>()->SetSprite(TEXTURE_NAME(item.png));
}
void CLifeItem::OnTriggerStay(CGameObject* other)
{
	// 1は一致。1超えは含まれている
	if (other->GetTagPtr()->Compare("PlayerDamage") == 1)
	{
		--m_nUseCnt;
		CSound::Play(eSE::SE_GET);
		CSound::SetVolume(eSE::SE_GET, 1.5f);

		if (m_nUseCnt <= 0)
		{
			CSound::Play(eSE::SE_GET);
			CSound::SetVolume(eSE::SE_GET, 1.5f);
			GetOwner()->SetState(CGameObject::E_ObjectState::TAKEOVER);
			m_pManager.lock()->SetStandby(BaseToDerived<CItemBase>());
		}
	}
}
//
CItemManager::CItemManager(std::shared_ptr<CGameObject> owner)
	:CComponent(owner)
{
}
void CItemManager::Awake()
{
	GetOwner()->GetTagPtr()->CreateTag(CItemManager::TAG_NAME);
	GetOwner()->SetName(CItemManager::TAG_NAME);
}
void CItemManager::Init()
{
	// ランダム生成
	int num = rand() % 20 + 30;
	Vector3 pos;
	for (int cnt = 0; cnt < num; ++cnt)
	{
		pos = Vector3(
			static_cast<float>(rand() % 250),
			static_cast<float>(0),
			static_cast<float>(rand() % 250)
		);
		Create(pos);
		
		//m_pItemList.back().lock()->SetState(CGameObject::E_ObjectState::TAKEOVER);
	}
}
void CItemManager::Update()
{
	if (m_pWaitItem.size() >= 10)
	{
		Vector3 pos;
		for (int cnt = 0; cnt < 10; ++cnt)
		{
			pos = Vector3(
				static_cast<float>(rand() % 250 - 150),
				static_cast<float>(0),
				static_cast<float>(rand() % 250 - 150)
			);
			Create(pos);
		}
	}
}
void CItemManager::Create(Vector3 pos, int cnt)
{
	if (m_pWaitItem.size() != 0)
	{
		// 最後尾を再利用
		auto item = m_pWaitItem.back();
		item.lock()->SetState(CGameObject::E_ObjectState::ACTIVE);
		item.lock()->Transform()->SetPos(pos);
		item.lock()->SetUseCnt(cnt);
		m_pWaitItem.pop_back();
		m_pItemList.push_back(item);
		return;
	}
	// 生成
	auto obj = CGameObject::CreateObject();
	obj.lock()->GetTransform()->SetPos(pos);
	m_pItemList.push_back(obj.lock()->AddComponent<CItemBase>());
	m_pItemList.back().lock()->GetOwner()->SetName("Item" + std::to_string(m_pItemList.size() + m_pWaitItem.size()));
	m_pItemList.back().lock()->SetUseCnt(cnt);
	m_pItemList.back().lock()->SetManager(BaseToDerived<CItemManager>());
}