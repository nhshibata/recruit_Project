//=========================================================
// �쐬:2022/04/19 (�Ηj��)
// �X�V:2022/09/20 �^�O�̔�r�Ɋ��S��v��������v���𔻕ʂł���悤�ύX
// �^�O : ���i�N���X
//=========================================================
#ifndef __TAG_H__
#define __TAG_H__

//--- �C���N���[�h��
#include <string>
#include <vector>
#include <CoreSystem/Util/cerealCommon.h>
#include <CoreSystem/system.h>

namespace MySpace
{
	namespace Game
	{
		using MySpace::System::CAppSystem;

		// ��{�I�ȃ^�O�����׽
		class CDefaultTagChar
		{
		public:
			static constexpr const char* DEFAULT = "Default";
			static constexpr const char* CAMERA = "MainCamera";
			static constexpr const char* LIGHT = "Light";
		};

		// �^�O�̑S�̊Ǘ�
		class CTagName : public CAppSystem<CTagName>
		{
			friend class CAppSystem<CTagName>;
			friend class CSingleton<CTagName>;
		private:
			std::vector<std::string> m_tagVec;
		private:
			CTagName();
			~CTagName();

		public:
			void LoadSystem();
			void SaveSystem();

			std::vector<std::string> GetList() 
			{
				return m_tagVec; 
			}

			int Find(std::string name)
			{
				int id = 0;
				for (std::vector<std::string>::iterator it = m_tagVec.begin(); it != m_tagVec.end(); ++it, ++id)
				{
					if ((*it) == name) { return id; }
				}
				return -1;
			}

			// �o�^���� / �K�v�Ȃ�
			bool CreateTag(std::string name) 
			{
				if (Find(name) == -1) { m_tagVec.push_back(name); return true; }
				return false;
			}

			std::string GetTag(int id)
			{
				if (m_tagVec.size() <= id) { return "null"; }return m_tagVec[id];
			}
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
			
		private:
			//--- �����o�֐�
			// ���
			inline int GetID(std::string name) { return CTagName::Get().Find(name); }
		public:
			CTag();
			CTag(std::string name);
			~CTag();

			// �o�^���� / �K�v�Ȃ�
			// �Ȃ���ΐ����B�ǂ���ɂ���o�^�����
			bool CreateTag(std::string name) 
			{ 
				bool ret = CTagName::Get().CreateTag(name);
				SetTag(name);
				return ret;
			}

			//--- �Z�b�^�[�E�Q�b�^�[
			inline std::string GetTag() { return CTagName::Get().GetTag(m_nTagID); }
			// ������ 
			// ���O��CreateTag�֐����Ăяo���K�v����
			// �o�^�A��������Ă��Ȃ����-1������
			inline void SetTag(std::string name) { m_nTagID = GetID(name); }

			// TODO: �G���[��������
			// *@�w��̃^�O���m�F
			// *@return (���S��v:1, ������v:2, ��v�Ȃ�:0)
			int Compare(std::string name)
			{
				if (auto tag = CTagName::Get().GetTag(m_nTagID); name == tag) { return true; }
				else if (tag.find(name) != std::string::npos) { return 2; }
				return false; 
			}

		};
	}
}

#endif  //__TAG_H__