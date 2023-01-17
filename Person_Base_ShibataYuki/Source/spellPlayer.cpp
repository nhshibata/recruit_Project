//========================================================
// [spellPlayer.cpp]
//------------------------
// �쐬:2023/01/02
//------------------------
//
//========================================================

//--- �C���N���[�h��
#include <spellPlayer.h>
#include <spellDefine.h>
#include <CoreSystem/Input/input.h>
#include <GameSystem/Component/Transform/rigidbody.h>
#include <GameSystem/Component/Collision/boxCollision.h>
#include <GameSystem/Component/Renderer/modelRenderer.h>

using namespace Spell;

//========================================================
// �R���X�g���N�^
//========================================================
CSpellPlayer::CSpellPlayer()
	:m_fSpeed(1), m_fJump(1)
{
}

//========================================================
// �������t���R���X�g���N�^
//========================================================
CSpellPlayer::CSpellPlayer(CGameObject::Ptr owner)
	:CComponent(owner)
{
}

//========================================================
// �f�X�N�g���N�^
//========================================================
CSpellPlayer::~CSpellPlayer()
{
}

//========================================================
// �������Ăяo��
//========================================================
void CSpellPlayer::Awake()
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
void CSpellPlayer::Init()
{

}

//========================================================
// �X�V
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
// �����蔻��
//========================================================
void CSpellPlayer::OnCollisionEnter(CGameObject* obj)
{
	if (obj->GetTagPtr()->Compare(Spell::TAG_LAND))
	{
		m_bLand = true;
	}
}

//========================================================
// �����蔻��
//========================================================
void CSpellPlayer::OnCollisionStay(CGameObject* obj)
{
	if (obj->GetTagPtr()->Compare(Spell::TAG_LAND))
	{
		m_bLand = false;
	}
}