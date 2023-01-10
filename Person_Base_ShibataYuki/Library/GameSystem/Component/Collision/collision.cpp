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
#include <ImGui/imgui.h>

using namespace MySpace::Game;
using namespace MySpace::SceneManager;

//==========================================================
// 引数付きコンストラクタ
//==========================================================
CCollision::CCollision(std::shared_ptr<CGameObject> owner, bool trigger)
	:CComponent(owner), m_bIsTrigger(trigger), m_vOldPos(0, 0, 0),
	m_pOldStayList(0),m_pHitList(0), m_nSystemIdx(-1)
{
}

//==========================================================
// デストラクタ
//==========================================================
CCollision::~CCollision()
{
	CSceneManager::Get().GetCollisionSystem()->ExecutSystem(m_nSystemIdx);

	m_pOldStayList.clear();
	m_pHitList.clear();
	m_pExitList.clear();
}

//==========================================================
// 生成時呼び出し
//==========================================================
void CCollision::Awake()
{
}

//==========================================================
// 初期化
//==========================================================
void CCollision::Init()
{
	// 当たり判定の要請
	RequestCollision();

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
	m_nSystemIdx = CSceneManager::Get().GetCollisionSystem()->RegistToSystem(BaseToDerived<CCollision>());
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
	
	// トリガーがtrue / 衝突オブジェがtrue
	bool trigger = m_bIsTrigger | other->IsTrigger();

	// 衝突したオブジェクトのタグを保存
	//SetColTag(pObj->GetTagPtr()->GetTag());

	// Stay
	m_pHitList.push_back(otherObj);

	// Stay:以前のフレームで接触していたか確認
	//if (auto it = std::find(m_pOldStayList.begin(), m_pOldStayList.end(), otherObj.lock()); it != m_pOldStayList.end())
	bool found = false;
	for (auto & obj : m_pOldStayList)
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
			// 元の座標を格納
			//Transform()->SetPos(m_vOldPos);
			GetOwner()->OnCollisionStay(pObj);
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
			// 元の座標を格納
			//Transform()->SetPos(m_vOldPos);
			GetOwner()->OnCollisionEnter(pObj);
		}
		else
		{
			GetOwner()->OnTriggerEnter(pObj);
		}

#ifdef BUILD_MODE
	++m_nDebugEnterCnt;
#endif // BUILD_MODE

	}

	// 離れたオブジェクトの判定
	ColObjectUpdate();
}

//==========================================================
// 衝突状態確認
// 触れている→ Enter → 次も接している → stay保存 → 離れた Exit
//==========================================================
void CCollision::ColObjectUpdate()
{
	//--- 前フレームのリストを確認
	for (std::list<std::weak_ptr<CGameObject>>::iterator it = m_pOldStayList.begin(); it != m_pOldStayList.end(); ++it)
	{
		if (!(*it).lock()) // 削除されている可能性を鑑みて
			continue;

		// 格納されていない、離れた
		//if (auto found = std::find(m_pHitList.begin(), m_pHitList.end(), (*it)); found == m_pHitList.end())
		bool res = false;
		for (auto & obj : m_pHitList)
		{	
			if ((*it).lock() == (*it).lock())
			{	// 格納されていた
				res = true;
				break;
			}
		}
		if (res)
			continue;

		m_pExitList.push_back((*it));
#ifdef BUILD_MODE
		++m_nDebugExitCnt;
#endif // BUILD_MODE

	}

	// hitリストからStayリストへ格納
	m_pOldStayList = m_pHitList;

	// 次の判定のため、クリア
	m_pHitList.clear();
}

//==========================================================
// 衝突終了確認
// 離れた相手のことを教える
//==========================================================
bool CCollision::ExitTell()
{
	//--- EnterリストにStayオブジェがあれば除外
	for (std::list<std::weak_ptr<CGameObject>>::iterator it = m_pExitList.begin(); it != m_pExitList.end(); ++it)
	{
		CGameObject* pObj = (*it).lock().get();
		if (!pObj)continue;

		if (!m_bIsTrigger)
		{
			GetOwner()->OnCollisionExit(pObj);
		}
		else
		{
			GetOwner()->OnTriggerExit(pObj);
		}
	}
	m_pExitList.clear();
	return false;
}


#ifdef BUILD_MODE

void CCollision::ImGuiDebug()
{
	ImGui::Text(u8"現在当たり", m_pOldStayList.size());
	ImGui::Text(u8"Enter:", m_nDebugEnterCnt);
	ImGui::Text(u8"Stay:", m_nDebugStayCnt);
	ImGui::Text(u8"Exit:", m_nDebugExitCnt);
	ImGui::Text(u8"当たり判定用過去サイズ [x:%f][y:%f][z:%f]", &m_vOldScale);
	ImGui::Checkbox(u8"Trigger", (bool*)&m_bIsTrigger);
	ImGui::DragFloat3(u8"Center", (float*)m_vCenter);

	// 再初期化はここで行う
	m_nDebugEnterCnt = m_nDebugStayCnt = m_nDebugExitCnt = 0;

}

#endif // BUILD_MODE