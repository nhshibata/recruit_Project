//=========================================================
// [mage.cpp]
// �쐬:2022/09/29
//---------------------------------------------------------
// �L�����N�^�[
//=========================================================

//--- �C���N���[�h��
#include <mage.h>

using namespace Spell;

void CMage::Awake()
{
	// �^�O�̍쐬
	GetOwner()->GetTagPtr()->CreateTag(CMage::TAG_NAME);
	
	// �K�v�Ⱥ��߰��Ă̒ǉ�
	m_pCommand = AddComponent<CCommandSpell>(0);
	Status.accessor = AddComponent<CBattleStatus>(0);

}
void CMage::Init()
{
	// 
}
void CMage::Update()
{
	// �擾
	if (STSpell* pSpell = m_pCommand->Present(); pSpell)
	{
		STSpell spell = *pSpell;

		m_pCommand->DestroyPresent();
	}

	// �擾��������UI�֓n��

}