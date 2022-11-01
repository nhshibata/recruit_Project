//=========================================================
// [command.cpp]
// �쐬:2022/09/21
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h��
#include <commandSpell.h>

//--- �萔��`
#define ToAttribute(num)		static_cast<Spell::EAttributte>(num)

using namespace Spell;

void CCommandSpell::Awake()
{
	this->Considered.accessor = 0;

	{
		m_SpellMap[0];
		m_SpellMap[0].Create("one", EAttributte::RED, EType::ATK, 1, std::vector<int>({ VK_U, VK_I, VK_O }));
	}
}
void CCommandSpell::Init()
{
	// �擾
	m_pStock = GetComponent<CCommandStock>();
	if(!m_pStock)
		m_pStock = AddComponent<CCommandStock>(0);
}
void CCommandSpell::Update()
{
	// �m��
	//if (!CInput::GetKeyTrigger(VK_0))
	if (!Execution.accessor)
		return;
	Execution.accessor = false;

	int num = 0;
	auto buffer = m_pStock->GetBuffer();
	auto it = m_SpellMap.begin();
	for (; it != m_SpellMap.end(); ++it)	// spell�ƈ�v������̂�T��
	{
		num = SpellCheck(buffer, (*it).second.command);
		num = static_cast<int>((*it).second.command.size()) - num;
		if (num <= this->Considered.accessor) // �݂Ȃ������m�F
			break;
	}

	// �Y��������̂��Ȃ�(�~�X)
	if (it == m_SpellMap.end())
	{
		m_pStock->CommandReset();	// ���Z�b�g
		return;
	}
	
	// �������������̂ŏ���n��
	// �������A�\�����閼�́A����? ����?(�U���A�񕜁A�h��)
	if (m_pMagic)
		delete m_pMagic;
	m_pMagic = new STSpell((*it).second);
	m_pMagic->Set(SpellCheckConsidered(buffer, (*it).second.command));
	m_pStock->CommandReset();	// ���Z�b�g
	// ����SE��
}
void CCommandSpell::FixedUpdate()
{

}
// *@��v�񐔂�Ԃ�
int CCommandSpell::SpellCheck(std::vector<int> list, std::vector<int> check)
{
	int ret = 0;	// ��v��
	int bufferSize = static_cast<int>(list.size());
	for (int cnt = 0; cnt < static_cast<int>(check.size()); ++cnt)
	{
		if (cnt >= bufferSize)
			break;
		if (check[cnt] != list[cnt])
			continue;
		++ret;
	}
	return ret;
}
// *@��v�󋵂�Ԃ�
std::vector<bool> CCommandSpell::SpellCheckConsidered(std::vector<int> list, std::vector<int> check)
{
	std::vector<bool> ret;
	int bufferSize = static_cast<int>(list.size());
	for (int cnt = 0; cnt < static_cast<int>(check.size()); ++cnt)
	{
		if (cnt >= bufferSize)
			break;

		ret.push_back(check[cnt] == list[cnt]);
	}
	return ret;
}