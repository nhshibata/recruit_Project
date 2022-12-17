//=====================================================
// [mapSystemBase.h]
// �쐬:2022/11/9
//----------------------------
// ��ӂ̃C���f�b�N�X�ŊǗ����邽�߂̃n�b�V��map�׽
// �o�^�����ۂɃC���f�b�N�X��n���A�j�����ȂǂɃC���f�b�N�X��n��
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
			//--- �G�C���A�X
			using MAP = std::unordered_map<int, T>;

		protected:
			//--- �����o�ϐ�
			MAP m_aIntMap;
			int m_nRegistCnt = 0;

		public:
			//--- �����o�֐�
			CMapSystemBase() { m_aIntMap.clear(); };
			virtual ~CMapSystemBase() { m_aIntMap.clear(); };

			// *�o�^
			// *�o�^�����ԍ���Ԃ�
			_NODISCARD inline int RegistToSystem(T type)
			{
				int ret = static_cast<int>(m_nRegistCnt);
				m_aIntMap[ret] = type;
				++m_nRegistCnt;
				return ret;
			}
			// *�j��(map�̂��߁A����͂����Ȃ�)
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