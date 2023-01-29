//=====================================================
// [eventMessage.cpp]
// 作成:2022/11/01 
//---------------------
// イベント通知用
//=====================================================

#include <common/eventMessage.h>

using namespace Spell;


void CEventMessage::Update()
{
	if (m_bMessageOn)
	{
		m_MessageMap.clear();
	}

	if (m_MessageMap.size() != 0)
	{
		m_bMessageOn = true;
	}
}

std::list<std::string> CEventMessage::GetMessage()
{
	//return std::list<std::string>(this->m_MessageMap.begin(),this->m_MessageMap.end());
	// TODO
	return std::list<std::string>();
}

bool CEventMessage::Ask(std::string msg)
{
	return m_MessageMap.at(msg);
}

void CEventMessage::Request(std::string msg)
{
	++m_MessageMap[msg];
}
