//========================================================
// [actionPlayer.cpp]
//--------------------------------------------------------
// 作成:2023/03/17
//--------------------------------------------------------
//
//========================================================

//--- インクルード部
#include <GameSystem/Component/actionPlayer.h>
#include <GameSystem/Component/Transform/rigidbody.h>
#include <GameSystem/Component/Collision/boxCollision.h>
#include <GameSystem/Component/Renderer/modelRenderer.h>

#include <CoreSystem/Input/input.h>

using namespace MySpace::Game;

//========================================================
// コンストラクタ
//========================================================
CActionPlayer::CActionPlayer()
	:m_fSpeed(1), m_fJump(1), m_bLand(false)
{
}

//========================================================
// 引き数付きコンストラクタ
//========================================================
CActionPlayer::CActionPlayer(CGameObject::Ptr owner)
	:CComponent(owner),
	m_fSpeed(1), m_fJump(1), m_bLand(false)
{
}

//========================================================
// デスクトラクタ
//========================================================
CActionPlayer::~CActionPlayer()
{
}

//========================================================
// 生成時呼び出し
//========================================================
void CActionPlayer::Awake()
{
	//--- ｺﾝﾎﾟｰﾈﾝﾄ追加
	// 当たり判定
	auto box = AddComponent<CBoxCollision>();
	box->SetTrigger(false);

	// 描画
	auto model = AddComponent<CModelRenderer>();
	//model->SetModel(MODEL_PATH2());

	// リジッドボディ
	auto rb = AddComponent<CRigidbody>();
	rb->SetGravity(true);

}

//========================================================
// 初期化
//========================================================
void CActionPlayer::Init()
{
	

}

//========================================================
// 更新
//========================================================
void CActionPlayer::Update()
{
	auto pos = Transform()->GetPos();
	Vector3 move;

	if (CInput::GetKeyPress(VK_W))
	{
		move = Transform()->forward() * m_fSpeed;
		pos.x += move.x;
		pos.z += move.z;
	}
	if (CInput::GetKeyPress(VK_S))
	{
		move = -Transform()->forward() * m_fSpeed;
		pos.x += move.x;
		pos.z += move.z;
	}
	if (CInput::GetKeyPress(VK_A))
	{
		move = -Transform()->up() * m_fSpeed;
		pos.x += move.x;
		pos.z += move.z;
	}
	if (CInput::GetKeyPress(VK_D))
	{
		move = Transform()->up() * m_fSpeed;
		pos.x += move.x;
		pos.z += move.z;
	}

	if (m_bLand)
	{
		if (CInput::GetKeyTrigger(VK_SPACE))
		{
			auto rb = GetComponent<CRigidbody>();
			rb->AddForce(Vector3(0, m_fJump, 0));
		}
	}

	Transform()->SetPos(pos);
}

//========================================================
// 当たり判定
//========================================================
void CActionPlayer::OnCollisionEnter(CGameObject* obj)
{
	if (obj->GetTagPtr()->Compare("Land"))
	{
		m_bLand = true;
	}
}

//========================================================
// 当たり判定
//========================================================
void CActionPlayer::OnCollisionStay(CGameObject* obj)
{
	if (obj->GetTagPtr()->Compare("Land"))
	{
		m_bLand = false;
	}
}