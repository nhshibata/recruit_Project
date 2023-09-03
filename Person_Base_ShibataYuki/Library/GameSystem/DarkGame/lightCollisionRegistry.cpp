//==========================================================
// [lightCollisionRegistry.cpp]
//----------------------------------------------------------
// �쐬:2023/06/01
//----------------------------------------------------------
// ���C�g�͈̔͊m�F�p
// ��ʂȂ����肵����L���b�V������
//==========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/DarkGame/lightCollisionRegistry.h>

using namespace DarkGame;

//==========================================================
// �R���X�g���N�^
//==========================================================
CLightCollisionRegistry::CLightCollisionRegistry()
{

}

//==========================================================
// �R���X�g���N�^
//==========================================================
CLightCollisionRegistry::CLightCollisionRegistry(CGameObject::Ptr ptr)
	:CLightCollision(ptr)
{

}

//==========================================================
// �f�X�g���N�^
//==========================================================
CLightCollisionRegistry::~CLightCollisionRegistry()
{

}

//==========================================================
// ����������
//==========================================================
void CLightCollisionRegistry::Awake()
{
	CLightCollision::Awake();
}

//==========================================================
// �X�V
//==========================================================
void CLightCollisionRegistry::Update()
{

}

//==========================================================
// ���̃N���A
//==========================================================
void CLightCollisionRegistry::Refresh()
{
	m_aHitTag.clear();
}

//==========================================================
// �����𖞂����Ă��邩
//==========================================================
bool CLightCollisionRegistry::IsRange(std::string spec)
{
	// ��v����^�O��������
	for (const auto & tag : m_aHitTag)
	{
		if (tag == spec)
		{
			return true;
		}
	}
	return false;
}

//==========================================================
// �����𖞂������ꍇ��true
//==========================================================
bool CLightCollisionRegistry::Check(const CLightCollision& other)
{
	// ����
	bool hit = CLightCollision::Check(other);
	if (hit)
	{
		// �^�O���L�^
		m_aHitTag.push_back(other.GetOwner()->GetTag());
	}

	return hit;
}