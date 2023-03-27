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
#include <GameSystem/Component/Camera/camera.h>

#include <CoreSystem/Input/input.h>
#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Game;

//========================================================
// コンストラクタ
//========================================================
CActionPlayer::CActionPlayer()
	:m_rb(nullptr)
{
}

//========================================================
// 引き数付きコンストラクタ
//========================================================
CActionPlayer::CActionPlayer(CGameObject::Ptr owner)
	:CComponent(owner),
	m_fSpeed(1), m_fJump(70), m_bLand(false)
	, m_rb(nullptr)
{
}

//========================================================
// デスクトラクタ
//========================================================
CActionPlayer::~CActionPlayer()
{
}

//========================================================
// 読み込み時呼び出し
//========================================================
void CActionPlayer::OnLoad()
{
	//--- ｺﾝﾎﾟｰﾈﾝﾄ追加	

	// リジッドボディ
	auto rb = GetComponent<CRigidbody>();
	m_rb = rb;

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

	// リジッドボディ
	auto rb = AddComponent<CRigidbody>();
	rb->SetGravity(true);
	rb->SetResist(0.0f);
	m_rb = rb.get();

	// 描画
	auto model = AddComponent<CModelRenderer>();
	//model->SetModel(MODEL_PATH2());
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
			m_rb->AddForce(Vector3(0, m_fJump, 0));
		}
	}

	if (auto cam = CCamera::GetMain(); cam)
		cam->SetTarget(pos);
	
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
		m_bLand = true;
	}
}

//========================================================
// 当たり判定
//========================================================
void CActionPlayer::OnCollisionExit(CGameObject* obj)
{
	if (obj->GetTagPtr()->Compare("Land"))
	{
		m_bLand = false;
	}
}


#if BUILD_MODE

void CActionPlayer::ImGuiDebug()
{
	Debug::SetTextAndAligned("Jump");
	ImGui::DragFloat("##Jump", &m_fJump);

	Debug::SetTextAndAligned("Land");
	ImGui::Checkbox("##Land", &m_bLand);

}

#endif // BUILD_MODE
