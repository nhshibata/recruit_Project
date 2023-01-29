//=========================================================
// [polygonRenderer.h] 
// 作成: 2022/06/28
//---------------------------------------------------------
// 2Dポリゴン用。RectTransform強制。
// なければ追加するが、ない場合なにかしらの異常があると考えるべき
// 命名ミスしたかも?
//=========================================================

//--- インクルードガード
#ifndef __POLYGON_RENDERER_COMPONENT_H__
#define __POLYGON_RENDERER_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/Renderer/renderer.h>
#include <GameSystem/Component/Transform/rectTransform.h>
#include <GraphicsSystem/Texture/spriteAnimation.h>

namespace MySpace
{
	namespace Game
	{
		using namespace MySpace::Graphics;

		//--- クラス定義
		class CPolygonRenderer : public CRenderer
		{
		private:
			// シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("polygonRender", cereal::base_class<CRenderer>(this)),
					CEREAL_NVP(m_pSprite), CEREAL_NVP(m_pRectTransform), CEREAL_NVP(m_nZValue)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("polygonRender", cereal::base_class<CRenderer>(this)),
					CEREAL_NVP(m_pSprite), CEREAL_NVP(m_pRectTransform), CEREAL_NVP(m_nZValue)
				);
			}
		public:
			enum class EZValue : int
			{
				BG = 0,
				DEFAULT = 50,
				FOG = 1000,
			};

		private:
			CSpriteAnimation m_pSprite;	// 描画テクスチャ
			RectTransWeakPtr m_pRectTransform;
			int m_nZValue;

		public:
			CPolygonRenderer()
				:m_nZValue(0)
			{
			}
			CPolygonRenderer(std::shared_ptr<CGameObject> owner);
			CPolygonRenderer(const CPolygonRenderer&);
			virtual ~CPolygonRenderer();

			virtual void OnLoad();
			virtual void Awake();
			virtual void Init();
			virtual void Update();
			virtual bool Draw();

			inline RectTransSharedPtr GetRectTransform() { return m_pRectTransform.lock(); }
			inline CSpriteAnimation* GetSprite() { return &m_pSprite; }
			inline std::string GetImageName() { return m_pSprite.GetImageName(); }
			inline const int GetZ() { return m_nZValue; }

			inline void SetRectTransform(RectTransSharedPtr ptr) { m_pRectTransform = ptr; }
			inline void SetImageName(std::string name)
			{
				m_pSprite.SetImage(name);
			}
			void SetZ(const int z);
			void SetZ(const EZValue z);

#if BUILD_MODE
			virtual void ImGuiDebug();
#endif // BUILD_MODE

		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CPolygonRenderer)

#endif // !__POLYGON_RENDERER_COMPONENT_H__
