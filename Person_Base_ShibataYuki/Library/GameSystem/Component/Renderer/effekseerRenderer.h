//=========================================================
// [EffekseerRenderer.h] 
// �쐬: 2022/08/11
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __EFFEKSEER_RENDERER_COMPONENT_H__
#define __EFFEKSEER_RENDERER_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/Component/Renderer/renderer.h>
#include <GraphicsSystem/Manager/effectManager.h>


namespace MySpace
{
	namespace Game
	{
		using namespace MySpace::Graphics;
		using namespace MySpace::System;

		//--- �N���X��`
		class CEffekseerRenderer : public CRenderer
		{
		private:
			// �V���A���C�Y
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
			//--- �����o�ϐ�
			CEffekseer* m_pEffekseer;
			Effekseer::Handle m_nHandle;
			std::u16string m_EffectName;
			bool m_bLoop;
			float m_fAngle;

		public:
			//--- �����o�֐�
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
