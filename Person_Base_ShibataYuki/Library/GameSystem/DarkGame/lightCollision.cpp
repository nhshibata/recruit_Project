//==========================================================
// [lightCollision.cpp]
//----------------------------------------------------------
// 作成:2023/05/19 
// 
//==========================================================

//--- インクルード部
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/DarkGame/lightCollision.h>
#include <GameSystem/DarkGame/lightCollisionManager.h>
#include <GameSystem/Manager/lightManager.h>

using namespace DarkGame;

//==========================================================
// コンストラクタ
//==========================================================
CLightCollision::CLightCollision()
{

}

//==========================================================
// コンストラクタ
//==========================================================
CLightCollision::CLightCollision(CGameObject::Ptr ptr)
	:CComponent(ptr)
{

}

//==========================================================
// デストラクタ
//==========================================================
CLightCollision::~CLightCollision()
{

}

//==========================================================
// 読み込み処理
//==========================================================
void CLightCollision::OnLoad()
{
	// 追加
	m_pLight = GetComponent<CPointLight>()->BaseToDerived<CPointLight>();
}

//==========================================================
// 生成時処理
//==========================================================
void CLightCollision::Awake()
{
	// 追加
	m_pLight = AddComponent<CPointLight>();
}

//==========================================================
// 初期化処理
//==========================================================
void CLightCollision::Init()
{
	// 管理ｸﾗｽへの依頼
	auto obj = CGameObject::FindGameObjectWithTag(CLightCollisionManager::TAG);
	auto mgr = obj.lock()->GetComponent<CLightCollisionManager>();
	mgr.lock()->AddList(this);
}

//==========================================================
// 更新
//==========================================================
void CLightCollision::Update()
{

}

//==========================================================
// 確認
// 範囲内のライトのタグを配列で返す
//==========================================================
bool CLightCollision::Check(const CLightCollision& other)
{
	// 自身の情報
	const Vector3 Apos = this->Transform()->GetPos();
	/*const float Ar = m_pLight.lock()->GetRange();

	auto light = other.GetLightPtr();
	const Vector3 Bpos = light->Transform()->GetPos();
	const float Br = light->GetRange();*/

	const float Ar = 0;
	const Vector3 Bpos;
	const float Br = 0;

	float dx = Apos.x - Bpos.x;
	float dy = Apos.y - Bpos.y;
	float dz = Apos.z - Bpos.z;
	float dr = Ar + Br;
	// 範囲確認
	if (dx * dx + dy * dy + dz * dz <= dr * dr)
	{
		return true;
	}

	return false;
}