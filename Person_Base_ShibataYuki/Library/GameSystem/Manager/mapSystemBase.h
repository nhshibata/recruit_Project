//=====================================================
// [mapSystemBase.h]
// �쐬:2022/11/9
//----------------------------
//
//=====================================================

//--- �C���N���[�h�K�[�h
#ifndef __MAP_SYSTEM_BASE_H__
#define __MAP_SYSTEM_BASE_H__

//--- �C���N���[�h��
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
			using MAP = std::unordered_map<int, T>;

		protected:
			MAP m_intMap;
			int m_nRegistCnt = 0;

		public:
			CMapSystemBase() {};
			virtual ~CMapSystemBase() { m_intMap.clear(); };

			// *�o�^
			// *�o�^�����ԍ���Ԃ�
			_NODISCARD inline int RegistToSystem(T type)
			{
				int ret = static_cast<int>(m_nRegistCnt);
				m_intMap[ret] = type;
				++m_nRegistCnt;
				return ret;
			}
			// *�j��(map�̂��߁A����͂����Ȃ�)
			virtual inline T ExecutSystem(int idx)
			{
				T ret = m_intMap[idx];
				m_intMap.erase(idx);
				return ret;
			}
		};
	}
}
#pragma endregion

#endif //__MAP_SYSTEM_BASE_H__