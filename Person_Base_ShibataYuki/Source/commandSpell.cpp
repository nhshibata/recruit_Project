//=========================================================
// [command.cpp]
// 作成:2022/09/21
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <commandSpell.h>

//--- 定数定義
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
	// 取得
	m_pStock = GetComponent<CCommandStock>();
	if(!m_pStock)
		m_pStock = AddComponent<CCommandStock>(0);
}
void CCommandSpell::Update()
{
	// 確定
	//if (!CInput::GetKeyTrigger(VK_0))
	if (!Execution.accessor)
		return;
	Execution.accessor = false;

	int num = 0;
	auto buffer = m_pStock->GetBuffer();
	auto it = m_SpellMap.begin();
	for (; it != m_SpellMap.end(); ++it)	// spellと一致するものを探す
	{
		num = SpellCheck(buffer, (*it).second.command);
		num = static_cast<int>((*it).second.command.size()) - num;
		if (num <= this->Considered.accessor) // みなし条件確認
			break;
	}

	// 該当するものがない(ミス)
	if (it == m_SpellMap.end())
	{
		m_pStock->CommandReset();	// リセット
		return;
	}
	
	// 発動成功したので情報を渡す
	// 成功数、表示する名称、属性? 効果?(攻撃、回復、防御)
	if (m_pMagic)
		delete m_pMagic;
	m_pMagic = new STSpell((*it).second);
	m_pMagic->Set(SpellCheckConsidered(buffer, (*it).second.command));
	m_pStock->CommandReset();	// リセット
	// 成功SE音
}
void CCommandSpell::FixedUpdate()
{

}
// *@一致回数を返す
int CCommandSpell::SpellCheck(std::vector<int> list, std::vector<int> check)
{
	int ret = 0;	// 一致回数
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
// *@一致状況を返す
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