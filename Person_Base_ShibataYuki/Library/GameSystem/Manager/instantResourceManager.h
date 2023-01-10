//=========================================================
// [instantResourceManager.h]
//----------------------
// �쐬:2022/07/31
//----------------------
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
				archive(CEREAL_NVP(m_aTexList), CEREAL_NVP(m_aModelList)/*, CEREAL_NVP(m_EffectList)*/);
			}
			template <typename Archive>
			void load(Archive & archive, std::uint32_t const version)
			{
				archive(CEREAL_NVP(m_aTexList), CEREAL_NVP(m_aModelList)/*, CEREAL_NVP(m_EffectList)*/);
			}

		private:
			//--- �G�C���A�X
			using RESOURCE_LIST = std::vector<std::string>;
			using EFFECT_RESOURCE_LIST = std::vector<std::u16string>;
		
		private:
			//--- �����o�ϐ�
			RESOURCE_LIST m_aTexList;
			RESOURCE_LIST m_aModelList;
			EFFECT_RESOURCE_LIST m_aEffectList;

		public:
			//--- �����o�֐�
			void Load();
			void Save();
			void SceneUnload();

			inline RESOURCE_LIST GetTex() { return m_aTexList; }
			inline RESOURCE_LIST GetModel() { return m_aModelList; }
			inline EFFECT_RESOURCE_LIST GetEffect() { return m_aEffectList; }
		};
	}
}
#endif // !__RESOURCE_MANAGER_H__
