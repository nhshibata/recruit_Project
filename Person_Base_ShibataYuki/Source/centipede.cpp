//=========================================================
// [centipede.cpp] 
// 作成:2022/08/16
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <centipede.h>
#include <CoreSystem/Input/input.h>
#include <CoreSystem/Sound/Sound.h>
#include <GameSystem/Component/Collision/sphereCollision.h>
#include <DebugSystem/imGuiPackage.h>

namespace {

	const float PLAYER_SPEED = 0.5f;
	const float PLAYER_MAX_SPEED = 80.0f;
	const float PLAYER_ROTATE = 2.5f;
	const float GROW_SIZE = 1.05f;

}

CCentipedeBody::CCentipedeBody(std::shared_ptr<CGameObject> owner)
	:CComponent(owner), m_fLife(30)
{

}
void CCentipedeBody::Awake()
{
	GetOwner()->GetTagPtr()->CreateTag(CCentipedeBody::TAG_NORMAL);
	GetOwner()->GetTagPtr()->CreateTag(CCentipedeBody::TAG_DAMAGE);
	m_pSphereCol = GetOwner()->AddComponent<CSphereCollision>();
	m_pSphereCol.lock()->SetTrigger(false);

	m_pModel = GetOwner()->AddComponent<CModelRenderer>();
	m_pModel.lock()->SetModel(CHARACTER_PATH(mukade_body.obj));
	Transform()->SetScale(Vector3(1.f, 1.f, 1.f));
}
void CCentipedeBody::Init()
{
}
void CCentipedeBody::Update()
{
	if (CInput::GetKeyTrigger(VK_K))
	{
		GetOwner()->GetTagPtr()->SetTag(CCentipedeBody::TAG_NORMAL);
	}
	if (CInput::GetKeyRelease(VK_K) && m_fLife > 0)
	{
		GetOwner()->GetTagPtr()->SetTag(CCentipedeBody::TAG_DAMAGE);
	}
}
void CCentipedeBody::SetLife(float hp)
{
	m_fLife = hp;
	m_pSphereCol.lock()->SetTrigger(false);
}
void CCentipedeBody::OnCollisionEnter(CGameObject* other)
{
	if (auto tag = GetOwner()->GetTagPtr(); tag->Compare("PlayerDamage") == 1)
	{
		if (other->GetTagPtr()->Compare("Item") == 1)
		{	// 成長
			m_pHead.lock()->AddGauge(1.0f);
		}
	}
	else if(tag->Compare("Player"))
	{
		if (other->GetTagPtr()->Compare("LifeItem") == 1)
		{	// 成長
			++m_fLife;
		}
		if (other->GetTagPtr()->Compare("Enemy"))
		{	// ダメージ
			--m_fLife;

			// ゲームーバー
			if (m_fLife < 0)
			{
				m_pSphereCol.lock()->SetTrigger(true);
				GetOwner()->GetTagPtr()->SetTag(CCentipedeBody::TAG_NORMAL);
				CSound::Play(eSE::SE_MISS);
			}
		}
	}
	// 共通
	if (other->GetTagPtr()->Compare("Bullet"))
	{	// ダメージ
		--m_fLife;

		// ゲームーバー
		if (m_fLife < 0)
		{	// 当たり判定をOFF
			m_pSphereCol.lock()->SetTrigger(true);
			GetOwner()->GetTagPtr()->SetTag(CCentipedeBody::TAG_NORMAL);
			CSound::Play(eSE::SE_MISS);
		}
	}
}
void CCentipedeBody::OnCollisionStay(CGameObject* other)
{
	//if (other->GetTagPtr()->Compare("Enemy") || other->GetTagPtr()->Compare("Bullet"))
	//{	// ダメージ
	//	--m_fLife;
	//	if (m_fLife < 0)
	//	{
	//		// 待機状態に変える
	//		GetOwner()->SetState(CGameObject::E_ObjectState::TAKEOVER);
	//		m_pHead.lock()->SetStanby(BaseToDerived<CCentipedeBody>());
	//	}
	//}
}
void CCentipedeBody::OnTriggerEnter(CGameObject* other)
{
	if (other->GetState() != CGameObject::E_ObjectState::ACTIVE)
		return;

	if (other->GetTagPtr()->Compare("Item"))
	{
#ifdef _DEBUG
		auto name = other->GetName();
#endif // _DEBUG

		// 値を変更?
		m_pHead.lock()->AddGauge(1);
		SetLife(m_fLife + 3);
		//Growth();
	}
}
void CCentipedeBody::OnTriggerStay(CGameObject* other)
{
	if (other->GetState() != CGameObject::E_ObjectState::ACTIVE)
		return;

	if (other->GetTagPtr()->Compare("Item"))
	{
#ifdef _DEBUG
		auto name = other->GetName();
#endif // _DEBUG

		m_pHead.lock()->AddGauge(1);
		//Growth();
	}
}

//=========================================================
CCentipede::CCentipede(std::shared_ptr<CGameObject> owner)
	:CComponent(owner), m_fSize(2), m_fLife(50),m_fGrowGauge(0),m_fNextGrowGauge(1) , m_fMoveSpeed(5), m_fMaxLife(50)
{
}
void CCentipede::Awake()
{
	GetOwner()->SetName("PlayerManager");
	GetOwner()->GetTagPtr()->CreateTag("Player");
	Transform()->SetPos(Vector3(10,0,0));

	// 初期の体
	int nInitNum = 5;
	for (int cnt = 0; cnt < nInitNum; ++cnt)
	{
		auto obj = CGameObject::CreateObject().lock();
		m_pBodyList.push_back(obj->AddComponent<CCentipedeBody>());
		m_pBodyList.back().lock()->GetOwner()->SetName("Centipede" + std::to_string(m_pBodyList.size()));
		m_pBodyList.back().lock()->SetHeadPtr(BaseToDerived<CCentipede>());

		m_pModelCom = obj->GetComponent<CModelRenderer>();
		if (0 == cnt || cnt == nInitNum - 1) 
		{
			m_pModelCom.lock()->SetModel(CHARACTER_PATH(mukade_tail.obj));		// 尾モデル読み込み用
			m_pTailModel = m_pModelCom.lock()->GetModel();						// ポインタ獲得
		}
		else
		{
			m_pModelCom.lock()->SetModel(CHARACTER_PATH(mukade_body.obj));		// 体モデル読み込み用
			m_pBodyModel = m_pModelCom.lock()->GetModel();						// ポインタ獲得
		}
	}
	// モデルのサイズを取得
	m_fSize = m_pModelCom.lock()->GetModel().lock()->GetRadius();

	// 座標をあらかじめ格納
	auto pos = Transform()->GetPos();
	auto rot = Transform()->GetRot();
	for (int cnt = 0; cnt < nInitNum * 2; ++cnt)
	{
		auto dir = Transform()->forward();
		pos.x += dir.x * (m_fMoveSpeed * CFps::Get().DeltaTime() * 60);
		pos.z += dir.z * (m_fMoveSpeed * CFps::Get().DeltaTime() * 60);
		
		// 頭部が一定距離移動した時、ポイント格納
		if (auto dist = Vector3::Length(m_vUpdatePoint, pos); dist > m_fSize)
		{
			m_vUpdatePoint = pos;
			m_vBodyParam.push_front(ST_Param(m_vUpdatePoint, rot));
			auto size = m_pBodyList.size();
			m_vBodyParam.resize(size);
		}
	}

	auto it = m_vBodyParam.begin();
	for (int cnt = 0; cnt < nInitNum; ++cnt, ++it)
	{
		m_pBodyList[cnt].lock()->Transform()->SetPos((*it).pos);
		m_pBodyList[cnt].lock()->Transform()->SetRot((*it).rot);
	}
	Transform()->SetPos(pos);

	m_vBodyParam.push_front(ST_Param(Transform()->GetPos(),Transform()->GetRot()));
}
void CCentipede::Init()
{
	/*if(m_pModelCom.lock())
		m_fSize = m_pModelCom.lock()->GetModel().lock()->GetRadius();*/
}
void CCentipede::Update()
{
	using namespace MySpace::Input;

	auto pos = Transform()->GetPos();
	auto rot = Transform()->GetRot();

	// 頭の向きを変更
	if (CInput::GetKeyPress(VK_A))
	{
		rot.y -= PLAYER_ROTATE;
		if (rot.y < -180.0f)
			rot.y += 360.0f;
	}
	if (CInput::GetKeyPress(VK_D))
	{
		rot.y += PLAYER_ROTATE;
		if (rot.y > 180.0f)
			rot.y -= 360.0f;
	}
	// 移動速度変更
	if (CInput::GetKeyTrigger(VK_S))
	{
		m_fMoveSpeed *= 0.9f;
		if (m_fMoveSpeed < PLAYER_SPEED)
			m_fMoveSpeed = PLAYER_SPEED;
	}
	if (CInput::GetKeyPress(VK_W))
	{
		m_fMoveSpeed += PLAYER_SPEED;
		if (m_fMoveSpeed > PLAYER_MAX_SPEED)
			m_fMoveSpeed = PLAYER_MAX_SPEED;
	}
	
	// 向いている方向に進む
	auto dir = Transform()->forward();
	pos.x +=  dir.x * (m_fMoveSpeed * CFps::Get().DeltaTime());
	pos.z +=  dir.z * (m_fMoveSpeed * CFps::Get().DeltaTime());

	Transform()->SetPos(pos);
	Transform()->SetRot(rot);

	// ゲージが一定に達したら
	if (m_fGrowGauge > m_fNextGrowGauge)
	{
		m_fGrowGauge = 0;
		m_fNextGrowGauge += 1;
		Growth();
	}
	
	// 頭部が一定距離移動した時、ポイント格納
	if (auto dist = Vector3::Length(m_vUpdatePoint, pos); dist > m_fSize)
	{
		m_vUpdatePoint = pos;
		m_vBodyParam.push_front(ST_Param(m_vUpdatePoint, rot));
		auto size = m_pBodyList.size();
		/*newPoint.x = sinf(rot.y) * m_fMoveSpeed;
		newPoint.z = cosf(rot.y) * m_fMoveSpeed;*/
		m_vBodyParam.resize(size);
	}
	else 
	{
		return;
	}

	PosUpdate();
}
void CCentipede::PosUpdate()
{
	// 座標をbodyに引き渡す
	auto it = m_vBodyParam.begin();
	auto bodySize = m_pBodyList.size();
	auto pointSize = m_vBodyParam.size();
	for (int cnt = 0; cnt < static_cast<int>(pointSize); ++cnt, ++it)
	{
		if (cnt >= bodySize || cnt >= pointSize)
			break;

		if (!m_pBodyList[cnt].lock())
		{
			continue;
		}
		m_pBodyList[cnt].lock()->Transform()->SetPos((*it).pos);
		// 最後尾は反転した角度を渡す
		m_pBodyList[cnt].lock()->Transform()->SetRot(
			(cnt == bodySize - 1 ? Vector3((*it).rot.x, (*it).rot.y + 180, (*it).rot.z) : (*it).rot)
		);
	}
}
void CCentipede::SetStanby(std::weak_ptr<CCentipedeBody> body)
{
	m_pStanbyList.push_back(body);
	// 除外
	for (auto it = m_pBodyList.begin(); it != m_pBodyList.end(); ++it)
		if ((*it).lock() == body.lock())
		{
			m_pBodyList.erase(it);
			break;
		}
}
void CCentipede::Growth()
{
	CSound::Play(eSE::SE_UP);

	// サイズを足す
	auto scale = Transform()->GetScale();
	Transform()->SetScale(Vector3(scale.x * GROW_SIZE, scale.y, scale.z * GROW_SIZE));
	m_fSize *= GROW_SIZE;

	// 体の数を増やす
	// body生成
	/*if (static_cast<int>(m_pStanbyList.size()) != 0)
	{
		m_pStanbyList.back().lock()->GetOwner()->SetState(CGameObject::E_ObjectState::ACTIVE);
		m_pBodyList.push_back(m_pStanbyList.back());
		m_pStanbyList.pop_back();
	}
	else*/
	{
		auto obj = CGameObject::CreateObject();
		obj.lock()->GetTransform()->SetPos(m_pBodyList.back().lock()->Transform()->GetPos());
		// 最後尾モデルを胴体に変更
		m_pBodyList.back().lock()->GetComponent<CModelRenderer>()->SetModel(m_pBodyModel);
		// オブジェにbodyｸﾗｽ追加
		m_pBodyList.push_back(obj.lock()->AddComponent<CCentipedeBody>() );
		// 名前
		m_pBodyList.back().lock()->GetOwner()->SetName("Centipede" + std::to_string(m_pBodyList.size()));
		m_pBodyList.back().lock()->SetHeadPtr(BaseToDerived<CCentipede>());
	}

	// 追加したオブジェに最後尾モデルに変更
	m_pBodyList.back().lock()->GetComponent<CModelRenderer>()->SetModel(m_pTailModel);
	for (auto & body : m_pBodyList)
	{
		body.lock()->Transform()->SetScale(scale);
	}
	PosUpdate();
}
void CCentipede::ImGuiDebug()
{
	ImGui::Text("speed[%f]", m_fMoveSpeed);
	ImGui::Text("life[%f]", m_fLife);
	ImGui::Text("standby[%d]", m_pStanbyList.size());
}