//=========================================================
// [billboardRenderer.h]
// 作成: 2022/07/04
//=========================================================

//--- インクルードガード
#ifndef __BILLBOARD_RENDERER_COMPONENT_H__
#define __BILLBOARD_RENDERER_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/Renderer/meshRenderer.h>
#include <GraphicsSystem/Texture/imageResource.h>
#include <GraphicsSystem/Texture/spriteAnimation.h>
#include <GraphicsSystem/Render/billboard.h>

namespace MySpace
{
	namespace Game
	{
		using namespace MySpace::Graphics;

		class CBillboardRenderer : public CMeshRenderer
		{
		private:
			// シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("meshRender", cereal::base_class<CMeshRenderer>(this)),
					CEREAL_NVP(m_pBillboard), CEREAL_NVP(m_pSprite)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("meshRender", cereal::base_class<CMeshRenderer>(this)),
					CEREAL_NVP(m_pBillboard), CEREAL_NVP(m_pSprite)
				);
			}
		private:
			//--- メンバ変数
			std::shared_ptr<CBillboard> m_pBillboard;
			std::shared_ptr<CSpriteAnimation> m_pSprite;

		public:
			//--- メンバ関数
			CBillboardRenderer() {};
			CBillboardRenderer(std::shared_ptr<CGameObject> owner);
			~CBillboardRenderer();

			virtual void Awake();
			virtual void Init();
			virtual void Update();
			virtual bool Draw();

			inline void SetSprite(std::shared_ptr<CSpriteAnimation> anim) { m_pSprite = anim; }
			inline void SetSprite(std::string name) { m_pSprite->SetImage(name); }
			inline CSpriteAnimation* GetSprite() { return m_pSprite.get(); }

#ifdef BUILD_MODE

			virtual void ImGuiDebug();

#endif // BUILD_MODE

		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CBillboardRenderer)

#endif // !__RENDERER_H_
