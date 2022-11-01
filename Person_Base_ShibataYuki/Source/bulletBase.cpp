//=========================================================
// [bulletBase.cpp] 
// 作成:2022/08/16
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <bulletBase.h>
#include <GameSystem/Component/Collision/sphereCollision.h>
#include <GameSystem/Component/Renderer/billboardRenderer.h>
#include <Application/screen.h>

CBulletBase::CBulletBase(std::shared_ptr<CGameObject> owner)
	:CComponent(owner),m_nAtk(1)
{

}
void CBulletBase::Awake()
{
	auto state = GetOwner()->GetState();

	GetOwner()->GetTagPtr()->CreateTag("Bullet");
	auto board = AddComponent<CBillboardRenderer>();
	board->SetSprite(TEXTURE_NAME(bullet.png));
	GetOwner()->SetLayer(static_cast<int>(CLayer::E_Layer::MODEL)+1);
	board->Transform()->SetScale({5,5,5});

	auto col = AddComponent<CSphereCollision>();
	col->SetTrigger(false);
}
void CBulletBase::Init()
{

}
void CBulletBase::Update()
{
	// 移動
	auto pos = Transform()->GetPos();
	pos += m_vVel;
	Transform()->SetPos(pos);

	// 画面外なら停止
	if (MySpace::System::CScreen::ScreenJudg(pos))
	{
		GetOwner()->SetState(CGameObject::E_ObjectState::TAKEOVER);
		m_pMgr.lock()->Standby(BaseToDerived<CBulletBase>());
	}
}
void CBulletBase::OnCollisionEnter(CGameObject* other)
{

}
void CBulletBase::OnCollisionStay(CGameObject* other)
{
	if (other->GetTagPtr()->Compare("Player"))
	{
		GetOwner()->SetState(CGameObject::E_ObjectState::TAKEOVER);
		m_pMgr.lock()->Standby(BaseToDerived<CBulletBase>());
	}
}
//
CBulletManager::CBulletManager(std::shared_ptr<CGameObject> owner)
	:CComponent(owner)
{

}
void CBulletManager::Awake()
{
	GetOwner()->GetTagPtr()->CreateTag(TAG_NAME);

	// 事前に確保しておく
	for (int cnt = 0; cnt < 10; ++cnt)
	{
		auto obj = CGameObject::CreateObject();
		obj.lock()->SetState(CGameObject::E_ObjectState::TAKEOVER);
		m_pStandby.push_back(obj.lock()->AddComponent<CBulletBase>());
		m_pStandby.back().lock()->SetMgr(BaseToDerived<CBulletManager>());
		m_pStandby.back().lock()->GetOwner()->SetName("bullet_" + std::to_string(m_pStandby.size()));
	}
}
void CBulletManager::Init()
{
}
void CBulletManager::Update()
{
}
void CBulletManager::Standby(std::weak_ptr<CBulletBase> ptr)
{
	m_pStandby.push_back(ptr);
	
	for (auto it = m_pBulletList.begin(); it != m_pBulletList.end(); ++it)
		if ((*it).lock() == ptr.lock()) 
		{
			m_pBulletList.erase(it);
			break;
		}
}
void CBulletManager::Create(Vector3 pos, Vector3 vel)
{
	if (m_pBulletList.size() > 30)
		return;

	if (m_pStandby.size() != 0)
	{
		// 最後尾を再利用
		auto bullet = m_pStandby.back();
		bullet.lock()->SetState(CGameObject::E_ObjectState::ACTIVE);
		bullet.lock()->Transform()->SetPos(pos);
		bullet.lock()->SetVel(vel);
		m_pStandby.pop_back();
		m_pBulletList.push_back(bullet);
		return;
	}

	auto obj = CGameObject::CreateObject();
	obj.lock()->GetTransform()->SetPos(pos);
	m_pBulletList.push_back(obj.lock()->AddComponent<CBulletBase>());
	m_pBulletList.back().lock()->SetVel(vel);
	m_pBulletList.back().lock()->SetMgr(BaseToDerived<CBulletManager>());
	m_pBulletList.back().lock()->GetOwner()->SetName("bullet_" + std::to_string(m_pBulletList.size() + m_pStandby.size()));
}

