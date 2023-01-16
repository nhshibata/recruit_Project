//=====================================================
// [mapSystemBase.h]
// 作成:2022/11/9
//----------------------------
// 一意のインデックスで管理するためのハッシュmapｸﾗｽ
// 登録した際にインデックスを渡し、破棄時などにインデックスを渡す
//=====================================================

//--- インクルードガード
#ifndef __MAP_SYSTEM_BASE_H__
#define __MAP_SYSTEM_BASE_H__

#define NOMINMAX

//--- インクルード部
#include <CoreSystem/Util/stl.h>
#include <limits>

#pragma region ForwardDeclaration
#pragma endregion

namespace MySpace
{
	namespace Game
	{
		template<class T>
		class CMapSystemBase
		{
		protected:
			//--- エイリアス
			using MAP = std::unordered_map<int, T>;

		protected:
			//--- メンバ変数
			MAP m_aIntMap;
			int m_nRegistCnt = 0;

		public:
			//--- メンバ関数
			CMapSystemBase() { m_aIntMap.clear(); };
			virtual ~CMapSystemBase() { m_aIntMap.clear(); };

			// *@登録
			// *@登録した際に番号を返す
			_NODISCARD inline int RegistToSystem(T type)
			{
				//int ret = static_cast<int>(m_nRegistCnt);
				//--- int型最大値
				int nMax = (std::numeric_limits<int>::max)();
				if(m_aIntMap.size() >= nMax)
					return -1;
				//--- 格納先探索
				int retIdx = 0;
				while (1)
				{
					retIdx = rand() % nMax;
					if (!m_aIntMap.count(retIdx))
						break;
				}
				m_aIntMap[retIdx] = type;
				++m_nRegistCnt;
				return retIdx;
			}

			// *@破棄(mapのため、整列はさせない)
			virtual inline T ExecutSystem(int idx)
			{
				T ret = m_aIntMap[idx];
				m_aIntMap.erase(idx);
				return ret;
			}

			// *@データ取得
			// *@生成時に渡したidからﾃﾞｰﾀを渡す
			inline T IDToData(int idx)
			{
				if (m_aIntMap.count(idx))
					return m_aIntMap[idx];
				return T();
			}

		};
	}
}

#endif //__MAP_SYSTEM_BASE_H__