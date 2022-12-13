//=========================================================
// [instantResourceManager.h]
// 作成:2022/07/31
// シーンのResourceを一時的に管理する
// セーブ・ロードを行う
//=========================================================

//--- インクルードガード
#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

//--- インクルード部
#include <CoreSystem/Util/stl.h>
#include <CoreSystem/Util/cerealCommon.h>

namespace MySpace
{
	namespace Game
	{
		class CInstantResourceManager
		{
		private:
			// シリアライズ用
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
			//--- メンバ変数
			RESOURCE_LIST m_TexList;
			RESOURCE_LIST m_ModelList;
			EFFECT_RESOURCE_LIST m_EffectList;

		public:
			//--- メンバ関数
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
