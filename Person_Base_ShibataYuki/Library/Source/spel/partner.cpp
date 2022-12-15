//=========================================================
// [partner.h]
// �쐬:2022/12/15
//---------------------------------------------------------
// �p�[�g�i�[���׽
//=========================================================

//--- �C���N���[�h��
#include <Source/spel/partner.h>
#include <AISystem/System/aiSystem.h>
#include <AISystem/nodeUtil.h>

using namespace Spell;

CPartner::CPartner(CGameObject::Ptr owner)
	:CComponent(owner)
{
	m_pAiSystem = AddComponent<CAISystem>();
	//m_pAiSystem.lock()->SetData(&m_Param);

	auto select1 = m_pAiSystem.lock()->MakeNode<CSelectorNode>();
	m_pAiSystem.lock()->AddNode(select1);
	


}

void CPartner::Awake()
{

}

void CPartner::Update()
{

}

