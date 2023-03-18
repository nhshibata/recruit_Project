//========================================================
// [actionPlayer.cpp]
//--------------------------------------------------------
// �쐬:2023/03/17
//--------------------------------------------------------
//
//========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/actionPlayer.h>
#include <GameSystem/Component/Transform/rigidbody.h>
#include <GameSystem/Component/Collision/boxCollision.h>
#include <GameSystem/Component/Renderer/modelRenderer.h>

#include <CoreSystem/Input/input.h>

using namespace MySpace::Game;

//========================================================
// �R���X�g���N�^
//========================================================
CActionPlayer::CActionPlayer()
	:m_fSpeed(1), m_fJump(1), m_bLand(false)
{
}

//========================================================
// �������t���R���X�g���N�^
//========================================================
CActionPlayer::CActionPlayer(CGameObject::Ptr owner)
	:CComponent(owner),
	m_fSpeed(1), m_fJump(1), m_bLand(false)
{
}

//========================================================
// �f�X�N�g���N�^
//========================================================
CActionPlayer::~CActionPlayer()
{
}

//========================================================
// �������Ăяo��
//========================================================
void CActionPlayer::Awake()
{
	//--- ���߰��Ēǉ�
	// �����蔻��
	auto box = AddComponent<CBoxCollision>();
	box->SetTrigger(false);

	// �`��
	auto model = AddComponent<CModelRenderer>();
	//model->SetModel(MODEL_PATH2());

	// ���W�b�h�{�f�B
	auto rb = AddComponent<CRigidbody>();
	rb->SetGravity(true);

}

//========================================================
// ������
//========================================================
void CActionPlayer::Init()
{
	

}

//========================================================
// �X�V
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
// �����蔻��
//========================================================
void CActionPlayer::OnCollisionEnter(CGameObject* obj)
{
	if (obj->GetTagPtr()->Compare("Land"))
	{
		m_bLand = true;
	}
}

//========================================================
// �����蔻��
//========================================================
void CActionPlayer::OnCollisionStay(CGameObject* obj)
{
	if (obj->GetTagPtr()->Compare("Land"))
	{
		m_bLand = false;
	}
}