//=========================================================
// [resourceMap.h]
// 作成:2022/08/01
//-----------------
// 後実装
// リソースをmap管理するｸﾗｽの抽象ｸﾗｽ
//=========================================================

//--- インクルードガード
#ifndef __RESOURCE_MAP_H__
#define __RESOURCE_MAP_H__

//--- インクルード部
#include <CoreSystem/Util/stl.h>

namespace MySpace
{
	namespace Graphics
	{
		template <class T1, class T2>
		class CResourceMap
		{
		public:
			//--- エイリアス
			using Map = std::map<T1, T2>;
			using Pair = std::pair<T1, T2>;

		protected:
			//--- メンバ変数
			Map m_aResourceMap;

		public:
			//--- メンバ関数
			virtual bool Load(T1 name) = 0;

			std::vector<T1> GetNameList() 
			{
				std::vector<T1> ret;
				for (auto it = m_aResourceMap.begin(); it != m_aResourceMap.end(); ++it)
				{
					ret.push_back((*it).first);
				}
				return ret;
			}

			// *@仮想関数
			virtual int SceneUnload() { return 0; };
		};
	}
}

#endif // !__RESOURCE_MAP_H__
