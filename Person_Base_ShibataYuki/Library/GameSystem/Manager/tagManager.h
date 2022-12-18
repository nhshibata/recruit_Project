//=========================================================
// [tagManager.h]
//--------------------
// �쐬:2022/12/18 tag.h���番��
//--------------------
//=========================================================

#ifndef __TAG_MANAGER_H__
#define __TAG_MANAGER_H__

//--- �C���N���[�h��
#include <string>
#include <vector>
#include <CoreSystem/Singleton.h>

namespace MySpace
{
	namespace Game
	{
		// �^�O�̑S�̊Ǘ�
		class CTagManager : public CSingleton<CTagManager>
		{
			friend class CSingleton<CTagManager>;
		private:
			//--- �����o�ϐ�
			static inline std::vector<std::string> m_aTagName;
			//std::vector<std::string> m_aTagName;

		private:
			//--- �����o�֐�
			CTagManager();
			~CTagManager();
		public:
			void LoadSystem();
			void SaveSystem();
			inline std::vector<std::string> GetNameList() 
			{
				return m_aTagName; 
			}

			// *@�C���f�b�N�X�擾
			_NODISCARD int FindIdx(std::string name)
			{
				int id = 0;
				for (std::vector<std::string>::iterator it = m_aTagName.begin(); it != m_aTagName.end(); ++it, ++id)
				{
					if ((*it) == name) 
						return id;
				}
				return -1;
			}

			// *@�^�O����(�o�^)
			// *@�o�^���� / �K�v�Ȃ�
			bool CreateRegist(std::string name) 
			{
				if (FindIdx(name) == -1) 
				{
					m_aTagName.push_back(name);
					return true;
				}
				return false;
			}

			// *@id����string�擾
			std::string IDToTag(int id)
			{
				if (m_aTagName.size() <= id) 
				{
					return "null";
				}
				return m_aTagName[id];
			}
		};

	}
}

#endif  //!__TAG_MANAGER_H__