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

//--- インクルード部
#include <CoreSystem/Util/stl.h>

#pragma region ForwardDeclaration
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

			// *登録
			// *登録した番号を返す
			_NODISCARD inline int RegistToSystem(T type)
			{
				int ret = static_cast<int>(m_nRegistCnt);
				m_aIntMap[ret] = type;
				++m_nRegistCnt;
				return ret;
			}
			// *破棄(mapのため、整列はさせない)
			virtual inline T ExecutSystem(int idx)
			{
				T ret = m_aIntMap[idx];
				m_aIntMap.erase(idx);
				return ret;
			}
		};
	}
}
#pragma endregion

#endif //__MAP_SYSTEM_BASE_H__