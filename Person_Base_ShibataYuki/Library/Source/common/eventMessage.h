//=====================================================
// [eventMessage.h]
// 作成:2022/11/01 
//---------------------
// イベント通知用
//=====================================================

#ifndef __EVENT_MESSAGE_H__
#define __EVENT_MESSAGE_H__

//--- インクルード部
#include <unordered_map>
#include <string>

namespace Spell
{
	class CEventMessage
	{
	private:
		//--- メンバ変数
		bool m_bMessageOn;
		std::unordered_map<std::string, int> m_MessageMap;

	public:
		//--- メンバ関数
		CEventMessage() 
			:m_bMessageOn(false)
		{};
		~CEventMessage() = default;
	
		// 1フレームだけ保持
		void Update();

		std::list<std::string> GetMessage();
		bool Ask(std::string msg);				// 希望のメッセージがあるか
		void Request(std::string);
	};
}

#endif // __EVENT_MESSAGE_H__
