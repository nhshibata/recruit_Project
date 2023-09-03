//=====================================================
// [mapSystemBase.h]
// �쐬:2022/11/9
// �X�V:2023/01/17 �^�ő�l���烉���_���Ȓl���C���f�b�N�X�����ɕύX
//----------------------------
// ��ӂ̃C���f�b�N�X�ŊǗ����邽�߂̃n�b�V��map�׽
// �o�^�����ۂɃC���f�b�N�X��n���A�j�����ȂǂɃC���f�b�N�X��n��
//=====================================================

//--- �C���N���[�h�K�[�h
#ifndef __MAP_SYSTEM_BASE_H__
#define __MAP_SYSTEM_BASE_H__

#define NOMINMAX

//--- �C���N���[�h��
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
			//--- �G�C���A�X
			using MAP = std::unordered_map<int, T>;

		protected:
			//--- �����o�ϐ�
			MAP m_aIntMap;

		public:
			//--- �����o�֐�
			CMapSystemBase() { m_aIntMap.clear(); };
			virtual ~CMapSystemBase() { m_aIntMap.clear(); };

			// *@�o�^�p�֐�
			// *@�o�^�����ۂɔԍ���Ԃ�
			_NODISCARD int RegistToSystem(T type)
			{
				//--- int�^�ő�l
				int nMax = 0;
				nMax = (std::numeric_limits<int>::max)();

				//--- �i�[�T�C�Y�m�F
				if(m_aIntMap.size() >= nMax)
					return -1;
				
				//--- �i�[��T��
				int retIdx = 0;
				while (1)
				{
					retIdx = rand() % nMax;
					if (!m_aIntMap.count(retIdx))
						break;
				}

				m_aIntMap[retIdx] = type;
				return retIdx;
			}

			// *@�j��(map�̂��߁A����͂����Ȃ�)
			virtual bool ExecutSystem(const int idx)
			{
				//--- ���o�^�m�F(���炩�ȃ~�X)
				if (idx == -1)
					return false;
				if (!m_aIntMap.count(idx))
					return false;

				m_aIntMap.erase(idx);
				return true;
			}

			// *@�f�[�^�擾
			// *@�������ɓn����id�����ް���n��
			inline T IDToData(int idx)
			{
				if (m_aIntMap.count(idx))
					return m_aIntMap[idx];
				return T();
			}

			inline MAP GetMap()const { return m_aIntMap; }

		};
	}
}

#endif //__MAP_SYSTEM_BASE_H__