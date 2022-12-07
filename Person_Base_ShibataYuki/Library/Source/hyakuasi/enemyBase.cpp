//=========================================================
// [enemyBase.h] 
// 作成:2022/08/16
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <enemyBase.h>
#include <centipede.h>
#include <itemBase.h>
#include <bulletBase.h>
#include <GameSystem/Component/Collision/sphereCollision.h>
#include <GameSystem/Component/Renderer/effekseerRenderer.h>
#include <GraphicsSystem/Manager/effectManager.h>
#include <DebugSystem/imguiManager.h>
#include <CoreSystem/Sound/Sound.h>


//--- 定数定義
namespace {
	const float DAMAGE_INTERVAL = 0.3f;
	const int LIFE_DAMAGE = 1;
	const float ENEMY_SPEED = 2.0f;
}

CEnemyBase::CEnemyBase(std::shared_ptr<CGameObject> owner)
	:CComponent(owner), m_nLife(5), m_nAtk(1), m_fDamageInterval(0)
{

}
void CEnemyBase::Awake()
{
	auto model = GetOwner()->AddComponent<CModelRenderer>();
	auto col = GetOwner()->AddComponent<CSphereCollision>();
	GetOwner()->GetTagPtr()->CreateTag("Enemy");
	Transform()->SetScale({ 5,5,5 });
	Transform()->SetRot({ 90,0,0 });
	model->SetModel(CHARACTER_PATH(05_12_zako3D.obj));
	col->SetTrigger(false);
	col->SetRadius(model->GetModel().lock()->GetRadius() * 9);
	
}
void CEnemyBase::Init()
{
	//auto model = GetOwner()->GetComponent<CModelRenderer>().lock();
	//auto col = GetOwner()->GetComponent<CSphereCollision>().lock();
}
void CEnemyBase::Update()
{
	auto tPos = m_pTarget.lock()->Transform()->GetPos();
	// 移動
	if (float dist = Vector3::Length(tPos, Transform()->GetPos()); dist != 0.0f)
	{
		Vector3 pos = Transform()->GetPos();
		Vector3 vDist = tPos - pos;
		pos.x = pos.x + vDist.x / dist * ENEMY_SPEED * CFps::Get().DeltaTime();
		pos.z = pos.z + vDist.z / dist * ENEMY_SPEED * CFps::Get().DeltaTime();
		Transform()->SetPos(pos);

		auto fAngle = -XMConvertToDegrees(atan2f(tPos.x - pos.x, tPos.z - pos.z));
		auto rot = Transform()->GetRot();
		Transform()->SetRot({ rot.x,fAngle,rot.z });
	}

	// ダメージインターバル
	if (m_fDamageInterval > 0)
		m_fDamageInterval-= CFps::Get().DeltaTime();
}
void CEnemyBase::OnCollisionEnter(CGameObject* other)
{
	// エフェクト発生?
	OnCollisionStay(other);
}
void CEnemyBase::OnCollisionStay(CGameObject* other)
{
	// ダメージ処理を行わない
	if (m_fDamageInterval > 0.0f)
		return;

	if (other->GetTagPtr()->Compare("Player"))
	{
		MySpace::Debug::ImGuiManager::Get().DebugLog("プレイヤー接触");
		CSound::Play(eSE::SE_HIT);
		CSound::SetVolume(eSE::SE_HIT, 1.5f);
		m_fDamageInterval = DAMAGE_INTERVAL;
		m_nLife -= LIFE_DAMAGE;
		if (m_nLife < 0)
		{
			CSound::Play(eSE::SE_EX);
			CSound::SetVolume(eSE::SE_EX, 1.5f);
			GetOwner()->SetState(CGameObject::E_ObjectState::TAKEOVER);
			// エフェクト発生
			// アイテム発生
			m_pManager.lock()->Standby(BaseToDerived<CEnemyBase>());
		}
	}
}

//
CBulletEnemy::CBulletEnemy(std::shared_ptr<CGameObject> owner)
	:CEnemyBase(owner),m_fFireInterval(1)
{
}
void CBulletEnemy::Update()
{
	static const float BULLET_SPEED = 1.1f;
	static const int BULLET_TIME = 5;

	// ダメージインターバル
	if (m_fDamageInterval > 0.0f)
		m_fDamageInterval -= CFps::Get().DeltaTime();
	// バレットインターバル
	if (m_fFireInterval > 0)
	{
		m_fFireInterval -= CFps::Get().DeltaTime();
		return;
	}

	//// 画面外ならしない
	//if (MySpace::System::CScreen::ScreenJudg(Transform()->GetPos()))
	//{
	//	return;
	//}

	// 弾発射
	if (float dist = Vector3::Length(m_pTarget.lock()->Transform()->GetPos(), Transform()->GetPos()); dist != 0.0f)
	{
		Vector3 pos = Transform()->GetPos();
		Vector3 vel;
		Vector3 vDist = m_pTarget.lock()->Transform()->GetPos() - pos;
		vel.x = vDist.x / dist * BULLET_SPEED;
		vel.z = vDist.z / dist * BULLET_SPEED;
		m_pManager.lock()->GetBullet()->Create(pos, vel);
		m_fFireInterval = BULLET_TIME + static_cast<float>(rand() % 3 + 1);
	}
}
void CBulletEnemy::OnCollisionEnter(CGameObject* other)
{
	CEnemyBase::OnCollisionEnter(other);
}
void CBulletEnemy::OnCollisionStay(CGameObject* other)
{
	CEnemyBase::OnCollisionStay(other);
}

//
CEnemyManager::CEnemyManager(std::shared_ptr<CGameObject> owner)
	:CComponent(owner), m_fCreateInterVal(10)
{
}
void CEnemyManager::Awake()
{
	m_fCreateCnt = m_fCreateInterVal;
	GetOwner()->GetTagPtr()->CreateTag("EnemyManager");

	CEffekseer::Get().Load(EFFECT_PATH + std::u16string(u"exe.efk"));
}
void CEnemyManager::Init()
{
	// この段階でなければおかしい
	m_pTarget = CGameObject::FindGameObjectWithTag("Player").lock()->GetComponent<CCentipede>();
	if(auto item = CGameObject::FindGameObjectWithTag(CItemManager::TAG_NAME).lock();item)
		m_pItemManager = item->GetComponent<CItemManager>();
	if (auto obj = CGameObject::FindGameObjectWithTag(CBulletManager::TAG_NAME); obj.lock())
		m_pBulletManager = obj.lock()->GetComponent<CBulletManager>();

	// 事前に確保
	for (int cnt = 0; cnt < 10; ++cnt)
	{
		auto obj = CGameObject::CreateObject();
		obj.lock()->SetState(CGameObject::E_ObjectState::TAKEOVER);
		m_pWaitList.push_back(obj.lock()->AddComponent<CBulletEnemy>());
		m_pWaitList.back().lock()->SetManager(BaseToDerived<CEnemyManager>());
		m_pWaitList.back().lock()->SetTarget(m_pTarget);
		m_pWaitList.back().lock()->GetOwner()->SetName("enemy" + std::to_string(m_pEnemyList.size() + m_pWaitList.size()));
	}

	// ランダム生成
	int num = 30;  // rand() % 20;
	float angle = 0;
	std::vector<Vector3> pos;
	for (int cnt = 0; cnt < num; ++cnt)
	{
		angle = static_cast<float>(rand() % 360 - 180);
		pos.push_back(Vector3(
			cosf(DirectX::XMConvertToRadians(angle)) * static_cast<float>(rand() % 100 + 30),
			static_cast<float>(0),
			sinf(DirectX::XMConvertToRadians(angle)) * static_cast<float>(rand() % 100 + 30))
		);
	}

	Create(pos, 0);
}
void CEnemyManager::Update()
{
	// 時間経過で生成、エネミーに変化
	
	// 生成インターバル
	if (m_fCreateCnt > 0)
	{
		m_fCreateCnt -= CFps::Get().DeltaTime();
	}
	
	if (m_pWaitList.size() < 10)
		return;

	// 生成
	if (m_fCreateCnt <= 0)
	{
		int num = CreateNum(m_fCreateInterVal);
		float angle = 0;
		std::vector<Vector3> pos;
		for (int cnt = 0; cnt < num; ++cnt)
		{
			angle = static_cast<float>(rand() % 360 - 180);
			pos.push_back(Vector3(
				cosf(DirectX::XMConvertToRadians(angle)) * static_cast<float>(rand() % 100 - 50),
				static_cast<float>(0),
				sinf(DirectX::XMConvertToRadians(angle)) * static_cast<float>(rand() % 100 - 50))
			);
		}

		Create(pos, 0);
		m_fCreateCnt = m_fCreateInterVal;
		//MySpace::Debug::ImGuiManager::Get().DebugLog("敵待機[" + std::to_string(static_cast<int>(m_pWaitList.size())) + "]");
	}
}
void CEnemyManager::Standby(std::weak_ptr<CEnemyBase> enemy)
{
	m_pWaitList.push_back(enemy);
	for (auto it = m_pEnemyList.begin(); it != m_pEnemyList.end(); ++it)
	{
		if ((*it).lock() == enemy.lock())
		{
			m_pEnemyList.erase(it);
			break;
		}
	}

	// アイテム生成依頼
	m_pItemManager.lock()->Create(enemy.lock()->Transform()->GetPos(), 8);

	// エフェクト再生
	auto obj = CGameObject::CreateObject();
	auto com = obj.lock()->AddComponent<CEffekseerRenderer>();
	obj.lock()->GetTransform()->SetPos(enemy.lock()->Transform()->GetPos());
	com->SetImageName(EFFECT_PATH + std::u16string(u"exe.efk"));
}
void CEnemyManager::Create(Vector3 pos)
{
	// 敵の種類もここで変更可
	if (m_pWaitList.size() != 0)
	{
		// 最後尾を再利用
		auto enemy = m_pWaitList.back();
		enemy.lock()->SetState(CGameObject::E_ObjectState::ACTIVE);
		enemy.lock()->SetLife(5);
		enemy.lock()->Transform()->SetPos(pos);
		m_pWaitList.pop_back();
		m_pEnemyList.push_back(enemy);
		return;
	}

	auto obj = CGameObject::CreateObject();
	obj.lock()->GetTransform()->SetPos(pos);
	m_pEnemyList.push_back(obj.lock()->AddComponent<CBulletEnemy>());
	m_pEnemyList.back().lock()->SetManager(BaseToDerived<CEnemyManager>());
	m_pEnemyList.back().lock()->SetTarget(m_pTarget);
	m_pEnemyList.back().lock()->GetOwner()->SetName("enemy" + std::to_string(m_pEnemyList.size() + m_pWaitList.size()));
}
void CEnemyManager::Create(std::vector<Vector3> posVec, int cnt)
{
	if (m_pEnemyList.size() > 40)
		return;

	// 先頭
	auto it = posVec.begin();

	// 生成
	Create(*it);

	// 再帰
	posVec.erase(it);
	if (posVec.size() != 0)
		Create(posVec, cnt + 1);
}
int CEnemyManager::CreateNum(float level)
{
	//float ret = 10;
	if (level < 10)
	{
		return 10;
	}
	if (level < 8)
	{
		return 15;
	}
	if (level < 5)
	{
		return 20;
	}
	if (level < 2)
	{
		return 30;
	}
	return 10;
}