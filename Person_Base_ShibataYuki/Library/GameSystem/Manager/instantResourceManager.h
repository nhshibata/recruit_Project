//=========================================================
// [instantResourceManager.h]
// �쐬:2022/07/31
// �V�[����Resource���ꎞ�I�ɊǗ�����
// �Z�[�u�E���[�h���s��
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

//--- �C���N���[�h��
#include <CoreSystem/Util/stl.h>
#include <CoreSystem/Util/cerealCommon.h>

namespace MySpace
{
	namespace Game
	{
		class CInstantResourceManager
		{
		private:
			// �V���A���C�Y�p
			friend cereal::access;
			template <typename Archive>
			void save(Archive & archive, std::uint32_t const version) const
			{
				archive(CEREAL_NVP(m_TexList), CEREAL_NVP(m_ModelList)/*, CEREAL_NVP(m_EffectList)*/);
			}
			template <typename Archive>
			void load(Archive & archive, std::uint32_t const version)
			{
				archive(CEREAL_NVP(m_TexList), CEREAL_NVP(m_ModelList)/*, CEREAL_NVP(m_EffectList)*/);
			}

			using RESOURCE_LIST = std::vector<std::string>;
			using EFFECT_RESOURCE_LIST = std::vector<std::u16string>;
		
		private:
			//--- �����o�ϐ�
			RESOURCE_LIST m_TexList;
			RESOURCE_LIST m_ModelList;
			EFFECT_RESOURCE_LIST m_EffectList;

		public:
			//--- �����o�֐�
			void Load();
			void Save();
			void SceneUnload();

			inline RESOURCE_LIST GetTex() { return m_TexList; }
			inline RESOURCE_LIST GetModel() { return m_ModelList; }
			inline EFFECT_RESOURCE_LIST GetEffect() { return m_EffectList; }
		};
	}
}
#endif // !__RESOURCE_MANAGER_H__
