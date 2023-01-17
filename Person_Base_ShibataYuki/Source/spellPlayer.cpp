//========================================================
// [spellPlayer.cpp]
//------------------------
// 作成:2023/01/02
//------------------------
//
//========================================================

//--- インクルード部
#include <spellPlayer.h>
#include <spellDefine.h>
#include <CoreSystem/Input/input.h>
#include <GameSystem/Component/Transform/rigidbody.h>
#include <GameSystem/Component/Collision/boxCollision.h>
#include <GameSystem/Component/Renderer/modelRenderer.h>

using namespace Spell;

//========================================================
// コンストラクタ
//========================================================
CSpellPlayer::CSpellPlayer()
	:m_fSpeed(1), m_fJump(1)
{
}

//========================================================
// 引き数付きコンストラクタ
//========================================================
CSpellPlayer::CSpellPlayer(CGameObject::Ptr owner)
	:CComponent(owner)
{
}

//========================================================
// デスクトラクタ
//========================================================
CSpellPlayer::~CSpellPlayer()
{
}

//========================================================
// 生成時呼び出し
//========================================================
void CSpellPlayer::Awake()
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
void CSpellPlayer::Init()
{

}

//========================================================
// 更新
//========================================================
void CSpellPlayer::Update()
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
		move = -Transform()->right() * m_fSpeed;
		pos.x += move.x;
		pos.z += move.z;
	}
	if (CInput::GetKeyPress(VK_D))
	{
		move = Transform()->right() * m_fSpeed;
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
void CSpellPlayer::OnCollisionEnter(CGameObject* obj)
{
	if (obj->GetTagPtr()->Compare(Spell::TAG_LAND))
	{
		m_bLand = true;
	}
}

//========================================================
// 当たり判定
//========================================================
void CSpellPlayer::OnCollisionStay(CGameObject* obj)
{
	if (obj->GetTagPtr()->Compare(Spell::TAG_LAND))
	{
		m_bLand = false;
	}
}