//=========================================================
// [tag.h]
//--------------------------
// �쐬:2022/04/19 (�Ηj��)
// �X�V:2022/09/20 �^�O�̔�r�Ɋ��S��v��������v���𔻕ʂł���悤�ύX
// �X�V:2022/12/18 �^�O�Ǘ��׽���t�@�C������
// �X�V:2022/12/22 �ÓI�����o�ϐ��Ń��X�g�Ǘ��ɕύX
//--------------------------
// �^�O : ���i�N���X
//=========================================================

#ifndef __TAG_H__
#define __TAG_H__

//--- �C���N���[�h��
#include <string>
#include <CoreSystem/Util/cerealCommon.h>

namespace MySpace
{
	namespace Game
	{
		// ��{�I�ȃ^�O�����׽
		class CTagDefault
		{
		public:
			static constexpr const char* DEFAULT =		"Default";
			static constexpr const char* MAIN_CAMERA =	"MainCamera";
			static constexpr const char* SUB_CAMERA =	"SubCamera";
			static constexpr const char* LIGHT =		"Light";
		};

		//--- �N���X��`
		class CTag
		{
		private:
			//--- �V���A���C�Y
			friend class cereal::access;
			template<class Archive>
			void save(Archive & archive) const
			{
				//archive(cereal::make_nvp(GetTag(), m_nTagID));
				archive(CEREAL_NVP(m_nTagID));
			}
			template<class Archive>
			void load(Archive & archive)
			{
				archive(CEREAL_NVP(m_nTagID));
			}
		private:
			//--- �����o�ϐ�
			int m_nTagID = 0;
			static inline std::vector<std::string> m_aTagName;
			
		private:
			//--- �����o�֐�

			// *@�C���f�b�N�X�擾
			_NODISCARD static int FindIdx(std::string name)
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
			static bool CreateRegist(std::string name)
			{
				if (FindIdx(name) == -1)
				{
					m_aTagName.push_back(name);
					return true;
				}
				return false;
			}

			// *@id����string�擾
			static std::string IDToTag(int id)
			{
				if (id < 0 || m_aTagName.size() <= id)
				{
					return "Unregistered";
				}
				return m_aTagName[id];
			}

		public:
			CTag();
			CTag(std::string name);
			~CTag();

			// *@�o�^���� / �K�v�Ȃ�
			// *@�Ȃ���ΐ���
			static bool CreateTag(std::string name) 
			{ 
				bool ret = CreateRegist(name);
				return ret;
			}

			//--- �Z�b�^�[�E�Q�b�^�[
			inline std::string GetTag() { return IDToTag(m_nTagID); }

			// *@������ 
			// *@���O��CreateTag�֐����Ăяo���K�v����
			// *@�o�^�A��������Ă��Ȃ����-1������
			inline void SetTag(std::string name) { m_nTagID = FindIdx(name); }

			// TODO: �G���[��������
			// *@�w��̃^�O���m�F
			// *@return (���S��v:1, ������v:2, ��v�Ȃ�:0)
			int Compare(std::string name)
			{
				if (auto tag = IDToTag(m_nTagID); name == tag) { return 1; }
				else if (tag.find(name) != std::string::npos) { return 2; }
				return 0; 
			}

			// *@���X�g�擾
			static inline std::vector<std::string> GetNameList() { return m_aTagName; }

			// *@�t�@�C���ۑ�
			static void SaveSystem();

			// *@�t�@�C���ǂݍ���
			static void LoadSystem();

#ifdef BUILD_MODE
			// *@ImGui�\��
			static void ImGuiTag(bool& disp);
#endif // BUILD_MODE

		};
	}
}

#endif  //__TAG_H__