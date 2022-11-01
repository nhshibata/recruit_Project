//=========================================================
// [resourceMap.h]
// �쐬:2022/08/01
// �����
// ���\�[�X��map�Ǘ�����׽�̒��۸׽
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __RESOURCE_MAP_H__
#define __RESOURCE_MAP_H__

//--- �C���N���[�h��
#include <CoreSystem/Util/stl.h>

namespace MySpace
{
	namespace Graphics
	{
		template <class T1, class T2>
		class CResourceMap
		{
		public:
			using Map = std::map<T1, T2>;
			using Pair = std::pair<T1, T2>;
		protected:
			Map m_ResourceMap;
		public:
			virtual bool Load(T1 name) = 0;

			std::vector<T1> GetNameList() 
			{
				std::vector<T1> ret;
				for (auto it = m_ResourceMap.begin(); it != m_ResourceMap.end(); ++it)
				{
					ret.push_back((*it).first);
				}
				return ret;
			}
		};
	}
}

#endif // !__RESOURCE_MAP_H__
