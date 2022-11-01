//=========================================================
// [command.h] 
// 作成:2022/09/21
//---------------------------------------------------------
//=========================================================

#ifndef __COMMAND_COMPONENT_H__
#define __COMMAND_COMPONENT_H__

#include <GameSystem/Component/component.h>
#include <CoreSystem/Input/input.h>
#include <CoreSystem/property.h>

namespace Spell
{
	// *@入力を記録するｸﾗｽ
	class CCommandStock : public CComponent
	{
	private:
		//--- メンバ変数
		std::vector<int> m_nInputList;		// 入力確認リスト
		std::vector<int> m_nInputBuffer;	// 入力を記録するリスト
	
	public:
		CProperty<float> InputInterval;
		CProperty<float> InputMaxInterval;

	public:
		//--- メンバ関数
		CCommandStock() {};
		CCommandStock(std::shared_ptr<CGameObject> owner):CComponent(owner)
		{};
		~CCommandStock() {};

		void Awake();
		void Init();
		void Update();
		void FixedUpdate();

		// *@入力情報初期化
		inline void CommandReset() { m_nInputBuffer.clear(); }
		inline auto GetBuffer() { return m_nInputBuffer; }
		inline bool SetKey(int key) {
			if (std::find(m_nInputList.begin(), m_nInputList.end(), key) == m_nInputList.end())
			{
				m_nInputList.push_back(key);
				return true;
			}
			return false;
		}
	};
}
#endif // !__COMMAND_COMPONENT_H__
