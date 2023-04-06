//========================================================
// [decoratorNode.h]
//------------------
// 作成:2022/12/04
//------------------
// 条件ノード(装飾)
// Decoratorパターン
//========================================================

#ifndef __DECORATOR_NODE_H__
#define __DECORATOR_NODE_H__

//--- インクルード部
#include <AISystem/Node/aiNode.h>

//--- 定数定義
#define AID_DEFINITION_IS_ACTION				bool IsExecute(AI::CAISystem* sys)			// 実行条件定義
#define AID_IMPLEMENTATION_IS_ACTION(type)		bool type##::IsExecute(AI::CAISystem* sys)	// 実行条件実装

namespace AI
{
	// 実行ノードを必ず設定
	class CDecoratorNode : public CAINode
	{
#pragma region Serialize
	private:
		//--- シリアライズ
		friend class cereal::access;
		template<class Archive>
		void save(Archive& archive) const
		{
			archive(cereal::make_nvp("decoratorNode", cereal::base_class<CAINode>(this)),
					CEREAL_NVP(m_pConcrete)
			);
		}
		template<class Archive>
		void load(Archive& archive)
		{
			archive(cereal::make_nvp("decoratorNode", cereal::base_class<CAINode>(this)),
					CEREAL_NVP(m_pConcrete)
			);
		}
#pragma endregion

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

CEREAL_REGISTER_TYPE(AI::CDecoratorNode)

#endif // !__DECORATOR_NODE_H__
