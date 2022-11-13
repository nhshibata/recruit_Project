//=========================================================
// [EffekseerRenderer.h] 
// 作成: 2022/08/11
//---------------------------------------------------------
//=========================================================

//--- インクルードガード
#ifndef __EFFEKSEER_RENDERER_COMPONENT_H__
#define __EFFEKSEER_RENDERER_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/Renderer/renderer.h>
#include <GraphicsSystem/Manager/effectManager.h>


namespace MySpace
{
	namespace Game
	{
		using namespace MySpace::Graphics;
		using namespace MySpace::System;

		//--- クラス定義
		class CEffekseerRenderer : public CRenderer
		{
		private:
			// シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("EffekseerRender", cereal::base_class<CRenderer>(this))
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("EffekseerRender", cereal::base_class<CRenderer>(this))
				);
			}
		private:
			//--- メンバ変数
			CEffekseer* m_pEffekseer;
			Effekseer::Handle m_nHandle;
			std::u16string m_EffectName;
			bool m_bLoop;
			float m_fAngle;

		public:
			//--- メンバ関数
			CEffekseerRenderer() {};
			CEffekseerRenderer(std::shared_ptr<CGameObject> owner);
			CEffekseerRenderer(const CEffekseerRenderer & copy) {
				this->m_pEffekseer = copy.m_pEffekseer;
				this->m_nHandle = copy.m_nHandle;
				this->m_EffectName = copy.m_EffectName;
				this->m_bLoop = copy.m_bLoop;
				this->m_fAngle = copy.m_fAngle;
			};
			~CEffekseerRenderer();

			virtual void Awake();
			virtual void Init();
			virtual void Update();
			virtual bool Draw();

			void SetImageName(std::u16string name);
			inline void SetLoop(bool flg) { m_bLoop = flg; }

#ifdef BUILD_MODE
		private:
			bool m_bRead = false;
		public:
			virtual void ImGuiDebug();
#endif // BUILD_MODE

		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CEffekseerRenderer)

#endif // !__RENDERER_H_
