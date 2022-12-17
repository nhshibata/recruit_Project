//========================================================
// [stateNode.h]
//-------------------
// 作成:2022/11/22
//-------------------
// ステート切替
// 
//========================================================

#ifndef __STATE_NODE_H__
#define __STATE_NODE_H__

//--- インクルード部
#include <AISystem/Node/aiNode.h>
#include <vector>

namespace AI
{

#pragma region ForwardDeclaration
	class CAISystem;
	class CStateCondition;
#pragma endregion

	class CStateNode : public CAINode
	{
	private:
		//--- エイリアス
		using StateCondition = std::shared_ptr<CStateCondition>;
		using StateMachine = std::vector<StateCondition>;

	private:
		//--- メンバ変数
		CAINode::Ptr m_pRoot;
		StateMachine m_pConditons;

	public:
		//--- メンバ関数

		// *@ノード実行関数
		// *@これをオーバーライドすること
		virtual CAINode::EStatus Update(CAISystem* system);

		// *@実行確認
		// *@子が居る限り再帰
		CAINode::EStatus Execute(CAISystem* system);

		// *@遷移追加
		inline void AddCondition(StateCondition add){ m_pConditons.push_back(add); }

		// *@ノードの設定
		inline void SetNode(CAINode::Ptr node) { m_pRoot = node; }
	};
}

#endif // !__STATE_NODE_H__

