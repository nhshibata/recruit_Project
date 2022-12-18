//=====================================================
// [eventMessage.h]
// 作成:2022/11/01 
//---------------------
// イベント通知用
//=====================================================

#ifndef __EVENT_MESSAGE_H__
#define __EVENT_MESSAGE_H__

//--- インクルード部
#include <CoreSystem/Singleton.h>
#include <unordered_map>
#include <string>

namespace Spell
{
	class CEventMessage : public MySpace::System::CSingleton<CEventMessage>
	{
		friend class MySpace::System::CSingleton<CEventMessage>;
	private:
		//--- メンバ変数
		bool m_bMessageOn;
		std::unordered_map<std::string, int> m_MessageMap;

	private:
		//--- メンバ関数
		CEventMessage() 
			:m_bMessageOn(false)
		{};
		~CEventMessage() = default;
	public:
		// 1フレームだけ保持
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
		bool Ask(std::string msg);				// 希望のメッセージがあるか
		void Request(std::string);
	};
}

#endif // __EVENT_MESSAGE_H__
