//=========================================================
//作成:2022/04/20 (水曜日)
// コリジョンｸﾗｽ(当たり判定 ) : 基底ｸﾗｽ
// 球とボックスの当たり判定は分けた方がいいかも?
//=========================================================

//--- インクルード部
#include <GameSystem/Component/Collision/collision.h>
#include <GameSystem/Manager/collisionSystem.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Manager/sceneManager.h>

#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Game;
using namespace MySpace::SceneManager;

//==========================================================
// コンストラクタ
//==========================================================
CCollision::CCollision()
	:m_nSystemIdx(-1)
{
}

//==========================================================
// 引数付きコンストラクタ
//==========================================================
CCollision::CCollision(std::shared_ptr<CGameObject> owner, bool trigger)
	:CComponent(owner), m_bIsTrigger(trigger), m_vOldPos(0, 0, 0),
	m_aOldHitList(0),m_aHitList(0), m_nSystemIdx(-1)
{
}

//==========================================================
// デストラクタ
//==========================================================
CCollision::~CCollision()
{
	//--- 登録解除
	if(m_nSystemIdx != -1)
		CSceneManager::Get()->GetCollisionSystem()->ExecutSystem(m_nSystemIdx);

	m_aOldHitList.clear();
	m_aHitList.clear();
	m_aExitList.clear();
}

//==========================================================
// 読み込み時呼び出し
//==========================================================
void CCollision::OnLoad()
{
	RequestCollision();
}

//==========================================================
// 生成時呼び出し
//==========================================================
void CCollision::Awake()
{
	// 当たり判定の要請
	RequestCollision();
}

//==========================================================
// 初期化
//==========================================================
void CCollision::Init()
{

	// 過去座標
	m_vOldPos = Transform()->GetPos();
	m_vOldScale = Transform()->GetScale();
	if (m_vOldScale.check({ 0,0,0 }))
	{
		m_vOldScale = { 1,1,1 };
	}
}

//==========================================================
// 更新
//==========================================================
void CCollision::Update()
{
	// 過去座標を更新
	m_vOldPos = Transform()->GetPos();

	// 当たり判定の要請
	//RequestCollision();
}

//==========================================================
// システムへの申請
//==========================================================
void CCollision::RequestCollision()
{
	if(m_nSystemIdx == -1)
		m_nSystemIdx = CSceneManager::Get()->GetCollisionSystem()->RegistToSystem(BaseToDerived<CCollision>());
}

//==========================================================
// 衝突時関数呼び出し
// 外部から呼び出される
// 当たった際の処理
// EnterとStayはここで判定
//==========================================================
void CCollision::HitResponse(CCollision* other)
{
	std::weak_ptr<CGameObject> otherObj = other->GetOwner(0);
	CGameObject* pObj = otherObj.lock().get();
	
	if (this == other)
		return;

	// トリガーがtrue / 衝突オブジェがtrue
	bool trigger = m_bIsTrigger | other->IsTrigger();

	// Enter
	m_aHitList.push_back(otherObj);

	// Stay:以前のフレームで接触していたか確認
	bool found = false;
	for (auto & obj : m_aOldHitList)
	{
		if (obj.lock() == otherObj.lock())
		{
			found = true;
			break;
		}
	}

	if (found)
	{
		if (!trigger)
		{
			GetOwner()->OnCollisionStay(pObj);
			pObj->OnCollisionStay(GetOwner());
		}
		else
		{
			GetOwner()->OnTriggerStay(pObj);
		}

#ifdef BUILD_MODE
		++m_nDebugStayCnt;
#endif // BUILD_MODE

	}
	else
	{	// Trigger:今のフレームで接触始めた
		if (!trigger)
		{
			GetOwner()->OnCollisionEnter(pObj);
			pObj->OnCollisionEnter(GetOwner());
		}
		else
		{
			GetOwner()->OnTriggerEnter(pObj);
		}

#ifdef BUILD_MODE
	++m_nDebugEnterCnt;
#endif // BUILD_MODE

	}

	
}

//==========================================================
// 衝突状態確認
// 触れている→ Enter → 次も接している → stay保存 → 離れた Exit
//==========================================================
void CCollision::ColObjectUpdate()
{
	//--- 前フレームのリストを確認
	for (std::list<std::weak_ptr<CGameObject>>::iterator it = m_aOldHitList.begin(); it != m_aOldHitList.end(); ++it)
	{
		if (!(*it).lock()) // 削除されている可能性を鑑みて
			continue;

		// 格納されていない、離れた		
		bool res = false;
		for (auto & obj : m_aHitList)
		{	
			if (obj.lock() == (*it).lock())
			{	// 格納されていた
				res = true;
				break;
			}
		}
		if (res)
			continue;

		m_aExitList.push_back((*it));

#ifdef BUILD_MODE
		++m_nDebugExitCnt;
#endif // BUILD_MODE

	}


}

//==========================================================
// 衝突終了確認
// 離れた相手のことを教える
//==========================================================
bool CCollision::ExitTell()
{
	// 離れたオブジェクトの判定
	ColObjectUpdate();

	for (std::list<std::weak_ptr<CGameObject>>::iterator it = m_aExitList.begin(); it != m_aExitList.end(); ++it)
	{
		CGameObject* pObj = (*it).lock().get();
		if (!pObj)
			continue;

		if (!m_bIsTrigger)
		{
			GetOwner()->OnCollisionExit(pObj);
		}
		else
		{
			GetOwner()->OnTriggerExit(pObj);
		}
	}

	// hitリストからStayリストへ格納
	m_aOldHitList.clear();
	m_aOldHitList = m_aHitList;
	// 次の判定のため、クリア
	m_aHitList.clear();
	m_aExitList.clear();
	
	return false;
}


#ifdef BUILD_MODE

void CCollision::ImGuiDebug()
{

	/*Debug::SetTextAndAligned("current hit");
	ImGui::Text("%d", m_aOldHitList.size());*/

	Debug::SetTextAndAligned("Enter");
	ImGui::Text("%d", m_nDebugEnterCnt);

	Debug::SetTextAndAligned("Stay");
	ImGui::Text("%d", m_nDebugStayCnt);

	Debug::SetTextAndAligned("Exit");
	ImGui::Text("%d", m_nDebugExitCnt);

	/*Debug::SetTextAndAligned(u8"当たり判定用過去サイズ");
	ImGui::Text("[x:%.5f][y:%.5f][z:%.5f]", &m_vOldScale);*/

	Debug::SetTextAndAligned("Is Trigger");
	ImGui::Checkbox("##Is Trigger", (bool*)&m_bIsTrigger);
	
	Debug::SetTextAndAligned("Center");
	ImGui::DragFloat3("##Center", (float*)m_vCenter);

	// 再初期化はここで行う
	/*m_nDebugEnterCnt = 0;*/
	m_nDebugStayCnt = 0;
	m_nDebugExitCnt = 0;

}

#endif // BUILD_MODE