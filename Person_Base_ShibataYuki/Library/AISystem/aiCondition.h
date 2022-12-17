//========================================================
// [aiCondition.h]
//------------------
// 作成:2022/11/22
//========================================================

#ifndef __AI_CONDITION_H__
#define __AI_CONDITION_H__

//--- インクルード部
#include <functional>
#include <vector>

namespace AI
{

#pragma region ForwardDeclaration
	class CAISystem;
#pragma endregion

	class CAICondition
	{
	public:
		//--- エイリアス
		// *@関数ポインタ
		// *@関数内は状態確認を行うものが望ましい
		using FuncPtr = bool(*)(CAISystem*);
		// *@ sharedﾎﾟｲﾝﾀ用
		using Ptr = std::shared_ptr<CAICondition>;

	private:
		using STLFunc = std::function<bool(CAISystem*)>;

	private:
		//--- ﾒﾝﾊﾞ変数
		std::vector<STLFunc> m_Functions;

	public:
		//--- メンバ関数

		// *@実行できるか関数確認
		// *@複数ある場合、全ての条件を満たさなければならない
		bool IsTransition(CAISystem* ptr)
		{
			for (auto & func : m_Functions)
				if (!func(ptr))
					return false;
			return true;
		}

		// *@確認条件追加
		// *@引き数: bool(*)(CAISystem*)
		inline void AddFunc(FuncPtr ptr)
		{
			m_Functions.push_back(ptr);
			//m_Functions.push_back(std::bind(ptr));
		}

		// *@初期化
		inline void Uninit()
		{
			m_Functions.clear();
		}

	};
}

#endif // !__CONDITION_H__
