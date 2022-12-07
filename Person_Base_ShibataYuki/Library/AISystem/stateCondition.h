//========================================================
// [stateCondition.h]
//------------------
// 作成:2022/12/04
//------------------
// 実行ノード
// 子は持てない
//========================================================

#ifndef __STATE_CONDITION_H__
#define __STATE_CONDITION_H__

//--- インクルード部
#include "aiCondition.h"

namespace AI
{
	class CAISystem;
	class CAINode;
	class CStateNode;

	class CStateCondition
	{
	private:
		using NextNode = std::shared_ptr<CStateNode>;

	private:
		//--- メンバ変数
		CAICondition::Ptr m_Condition;
		NextNode m_pNextNode;

	public:
		//--- メンバ関数
		CStateCondition(NextNode next): m_pNextNode(next)
		{
			m_Condition = std::make_shared<CAICondition>();
		}
		~CStateCondition()
		{}

		// *@状態確認
		NextNode CheckTransition(CAISystem* system)
		{
			// 自分が実行できる状態か確認
			if (m_Condition->IsTransition(system))
				return m_pNextNode;

			return NextNode();
		}

		// *@関数ﾎﾟｲﾝﾀ追加
		inline void AddCondition(CAICondition::FuncPtr func){ m_Condition->AddFunc(func); }

		// *@次へのノード
		inline void SetNextNode(NextNode node) { m_pNextNode = node; }
	};
}

#endif // !__STATE_CONDITION_H__
