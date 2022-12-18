//=========================================================
// [tag.h]
//--------------------------
// �쐬:2022/04/19 (�Ηj��)
// �X�V:2022/09/20 �^�O�̔�r�Ɋ��S��v��������v���𔻕ʂł���悤�ύX
// �X�V:2022/12/18 �^�O�Ǘ��׽���t�@�C������
//--------------------------
// �^�O : ���i�N���X
//=========================================================

#ifndef __TAG_H__
#define __TAG_H__

//--- �C���N���[�h��
#include <GameSystem/Manager/tagManager.h>
#include <string>
#include <CoreSystem/Util/cerealCommon.h>

namespace MySpace
{
	namespace Game
	{
		// ��{�I�ȃ^�O�����׽
		class CDefaultTagChar
		{
		public:
			static constexpr const char* DEFAULT = "Default";
			static constexpr const char* CAMERA = "MainCamera";
			static constexpr const char* LIGHT = "Light";
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
			inline int GetID(std::string name) { return CTagManager::Get()->FindIdx(name); }
		public:
			CTag();
			CTag(std::string name);
			~CTag();

			// *@�o�^���� / �K�v�Ȃ�
			// *@�Ȃ���ΐ����B�ǂ���ɂ���o�^�����
			bool CreateTag(std::string name) 
			{ 
				bool ret = CTagManager::Get()->CreateRegist(name);
				SetTag(name);
				return ret;
			}

			//--- �Z�b�^�[�E�Q�b�^�[
			inline std::string GetTag() { return CTagManager::Get()->IDToTag(m_nTagID); }
			// *@������ 
			// *@���O��CreateTag�֐����Ăяo���K�v����
			// *@�o�^�A��������Ă��Ȃ����-1������
			inline void SetTag(std::string name) { m_nTagID = GetID(name); }

			// TODO: �G���[��������
			// *@�w��̃^�O���m�F
			// *@return (���S��v:1, ������v:2, ��v�Ȃ�:0)
			int Compare(std::string name)
			{
				if (auto tag = CTagManager::Get()->IDToTag(m_nTagID); name == tag) { return true; }
				else if (tag.find(name) != std::string::npos) { return 2; }
				return false; 
			}

		};
	}
}

#endif  //__TAG_H__