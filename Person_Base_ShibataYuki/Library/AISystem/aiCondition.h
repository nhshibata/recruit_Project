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
#include <CoreSystem/Util/cerealCommon.h>

namespace AI
{

#pragma region ForwardDeclaration
	class CAISystem;
#pragma endregion

	class CAICondition
	{
#pragma region Serialize
	private:
		//--- シリアライズ
		friend class cereal::access;
		template<class Archive>
		void save(Archive& archive) const
		{
			archive(CEREAL_NVP(m_Functions)
			);
		}
		template<class Archive>
		void load(Archive& archive)
		{
			archive(CEREAL_NVP(m_Functions)
			);
		}
#pragma endregion
	public:
		//--- エイリアス
		// *@関数ポインタ
		// *@関数内は状態確認を行うものが望ましい
		//using FuncPtr = bool(*)(CAISystem*);
		using FuncPtr = std::function<bool(CAISystem*)>;

		// *@ sharedﾎﾟｲﾝﾀ用
		using Ptr = std::shared_ptr<CAICondition>;

	private:
		//--- ﾒﾝﾊﾞ変数
		std::list<FuncPtr> m_Functions;

	public:
		//--- メンバ関数

		// *@実行できるか関数確認
		// *@複数ある場合、全ての条件を満たさなければならない
		bool IsTransition(CAISystem* sys)const
		{
			for (auto & func : m_Functions)
				if (!func(sys))
					return false;
			return true;
		}

		// *@確認条件追加
		// *@引き数: bool(*)(CAISystem*)
		inline void AddFunc(FuncPtr ptr)
		{
			m_Functions.push_back(std::move(ptr));
		}

		// *@確認条件破棄
		void RemoveFunc(FuncPtr ptr)
		{
			for (auto it = m_Functions.begin(); it != m_Functions.end(); ++it)
			{
				// 同一のﾎﾟｲﾝﾀを探索
				if ((*it).target<bool(CAISystem*)>() == ptr.target<bool(CAISystem*)>())
				{
					m_Functions.erase(it);
					return;
				}
			}
		}

		// *@初期化
		inline void Uninit()
		{
			m_Functions.clear();
		}

	};
}

#endif // !__CONDITION_H__
