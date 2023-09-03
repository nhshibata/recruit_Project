//==========================================================
// [lightCollisionCondition.cpp]
//----------------------------------------------------------
// �쐬:2023/06/01
//----------------------------------------------------------
// ���C�g�͈̔͊m�F�p
// �������m�F���A�����ێ�����
//==========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/DarkGame/lightCollisionCondition.h>

using namespace DarkGame;

//==========================================================
// �R���X�g���N�^
//==========================================================
CLightCollisionCondition::CLightCollisionCondition()
	:m_bHit(false)
{

}

//==========================================================
// �R���X�g���N�^
//==========================================================
CLightCollisionCondition::CLightCollisionCondition(CGameObject::Ptr ptr)
	:CLightCollision(ptr), m_bHit(false)
{

}

//==========================================================
// �f�X�g���N�^
//==========================================================
CLightCollisionCondition::~CLightCollisionCondition()
{

}

//==========================================================
// ����������
//==========================================================
void CLightCollisionCondition::Awake()
{
	CLightCollision::Awake();
}

//==========================================================
// �X�V
//==========================================================
void CLightCollisionCondition::Update()
{

}

//==========================================================
// ���̃N���A
//==========================================================
void CLightCollisionCondition::Refresh()
{
	m_bHit = false;
}

//==========================================================
// �����𖞂����Ă��邩
//==========================================================
bool CLightCollisionCondition::IsRange(std::string spec)
{
	return m_bHit;
}

//==========================================================
// �����𖞂������ꍇ��true
//==========================================================
bool CLightCollisionCondition::Check(const CLightCollision& other)
{
	// �Ώۂ��^�O�����ƈ�v���邩
	if (other.GetOwner()->GetTagPtr()->Compare(m_SpecName))
	{
		// �͈͔���
		bool hit = CLightCollision::Check(other);
		m_bHit |= hit;

		return hit;
	}

	return false;
}