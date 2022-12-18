//=====================================================
// [eventMessage.h]
// �쐬:2022/11/01 
//---------------------
// �C�x���g�ʒm�p
//=====================================================

#ifndef __EVENT_MESSAGE_H__
#define __EVENT_MESSAGE_H__

//--- �C���N���[�h��
#include <CoreSystem/Singleton.h>
#include <unordered_map>
#include <string>

namespace Spell
{
	class CEventMessage : public MySpace::System::CSingleton<CEventMessage>
	{
		friend class MySpace::System::CSingleton<CEventMessage>;
	private:
		//--- �����o�ϐ�
		bool m_bMessageOn;
		std::unordered_map<std::string, int> m_MessageMap;

	private:
		//--- �����o�֐�
		CEventMessage() 
			:m_bMessageOn(false)
		{};
		~CEventMessage() = default;
	public:
		// 1�t���[�������ێ�
		void Update()
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

		std::list<std::string> GetMessage();
		bool Ask(std::string msg);				// ��]�̃��b�Z�[�W�����邩
		void Request(std::string);
	};
}

#endif // __EVENT_MESSAGE_H__
