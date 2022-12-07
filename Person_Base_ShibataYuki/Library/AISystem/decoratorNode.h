//========================================================
// [decoratorNode.h]
//------------------
// 作成:2022/12/04
//------------------
// 条件ノード(装飾)
// Decoratorパターン
//========================================================

//--- インクルード部
#include "aiNode.h"

#ifndef __DECORATOR_NODE_H__
#define __DECORATOR_NODE_H__

#define AID_DEFINITION_IS_ACTION				bool IsExecute(AI::CAISystem* sys)
#define AID_IMPLEMENTATION_IS_ACTION(type)		bool type##::IsExecute(AI::CAISystem* sys)

namespace AI
{
	class CDecoratorNode : public CAINode
	{
	private:
		//--- メンバ変数
		CAINode::Ptr m_pConcrete;		// 装飾対象

	public:
		//--- メンバ関数

		// *@条件確認
		// *@オーバーライドして、実行する条件を記述
		virtual bool IsExecute(CAISystem* sys) = 0;

		// *@ノード実行関数
		CAINode::EStatus Execute(CAISystem* sys)
		{
			if(!IsExecute(sys))
				return CAINode::EStatus::FAILURE;
			// 実行
			//if(m_pConcrete)
			return m_pConcrete->Execute(sys);
			
			return CAINode::EStatus::FAILURE;
		}

		// *@修飾対象設定
		inline void SetConcreate(CAINode::Ptr ptr) { m_pConcrete = ptr; }
	};
}

#endif // !__DECORATOR_NODE_H__
