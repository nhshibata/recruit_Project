//=====================================================
// [eventMessage.cpp]
// �쐬:2022/11/01 
//---------------------
// �C�x���g�ʒm�p
//=====================================================

#include <eventMessage.h>

using namespace Spell;

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
