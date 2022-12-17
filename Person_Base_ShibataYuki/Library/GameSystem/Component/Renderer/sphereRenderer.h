//=========================================================
// [sphereRenderer.h] 
// 作成: 2022/06/27
// 更新: 2022/12/10 インスタンシング描画対応
//---------------------------------------------------------
// 3D描画
//=========================================================

//--- インクルードガード
#ifndef __SPHERE_RENDERER_COMPONENT_H__
#define __SPHERE_RENDERER_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/Renderer/meshRenderer.h>
#include <GraphicsSystem/Render/Sphere.h>

namespace MySpace
{
	namespace Game
	{
		using namespace MySpace::Graphics;

		class CSphereRenderer : public CMeshRenderer
		{
		private:
			// シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("meshRender", cereal::base_class<CMeshRenderer>(this)),
					CEREAL_NVP(m_fRadius)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("meshRender", cereal::base_class<CMeshRenderer>(this)),
					CEREAL_NVP(m_fRadius)
				);
			}
		private:
			//--- メンバ変数
			std::shared_ptr<CSphere> m_pBSphere;
			float m_fRadius;
		public:
			//--- メンバ関数
			CSphereRenderer() :m_fRadius(100) {};
			CSphereRenderer(std::shared_ptr<CGameObject> ptr);
			virtual ~CSphereRenderer();

			virtual void Awake();
			virtual void Init();
			virtual void Update();
			virtual bool Draw();

			inline void SetRadius(float value) { m_fRadius = value; }
			HRESULT SetSphere(float radius);
			
#if BUILD_MODE

			void ImGuiDebug();

#endif // BUILD_MODE


		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CSphereRenderer)

#endif // !__BOX_MESH_H__
