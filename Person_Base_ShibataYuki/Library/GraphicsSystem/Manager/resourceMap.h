//=========================================================
// [resourceMap.h]
// �쐬:2022/08/01
//-----------------
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
			//--- �G�C���A�X
			using Map = std::map<T1, T2>;
			using Pair = std::pair<T1, T2>;

		protected:
			//--- �����o�ϐ�
			Map m_aResourceMap;

		public:
			//--- �����o�֐�
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

			// *@���z�֐�
			virtual int SceneUnload() { return 0; };
		};
	}
}

#endif // !__RESOURCE_MAP_H__
