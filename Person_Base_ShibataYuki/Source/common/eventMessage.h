//=====================================================
// [eventMessage.h]
// �쐬:2022/11/01 
//---------------------
// �C�x���g�ʒm�p
//=====================================================

#ifndef __EVENT_MESSAGE_H__
#define __EVENT_MESSAGE_H__

//--- �C���N���[�h��
#include <unordered_map>
#include <string>

namespace Spell
{
	class CEventMessage
	{
	private:
		//--- �����o�ϐ�
		bool m_bMessageOn;
		std::unordered_map<std::string, int> m_MessageMap;

	public:
		//--- �����o�֐�
		CEventMessage() 
			:m_bMessageOn(false)
		{};
		~CEventMessage() = default;
	
		// 1�t���[�������ێ�
		void Update();

		std::list<std::string> GetMessage();
		bool Ask(std::string msg);				// ��]�̃��b�Z�[�W�����邩
		void Request(std::string);
	};
}

#endif // __EVENT_MESSAGE_H__