

#ifndef __BOX_RENDERER_COMPONENT_H__
#define __BOX_RENDERER_COMPONENT_H__

#include <GraphicsSystem/Render/box.h>
#include <GameSystem/Component/Renderer/meshRenderer.h>


namespace MySpace
{
	using namespace MySpace::MyMath;
	using namespace MySpace::Graphics;

	namespace Game
	{
		class CBoxRenderer :public CMeshRenderer
		{
		private:
			// シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("meshRender", cereal::base_class<CMeshRenderer>(this)),
					CEREAL_NVP(m_pBBox), CEREAL_NVP(m_vSize)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("meshRender", cereal::base_class<CMeshRenderer>(this)),
					CEREAL_NVP(m_pBBox), CEREAL_NVP(m_vSize)
				);
			}
		private:
			std::shared_ptr<CBox> m_pBBox;
			Vector3 m_vSize;
		public:
			CBoxRenderer() :m_vSize(100, 100, 100) {};
			CBoxRenderer(std::shared_ptr<CGameObject> ptr);
			virtual ~CBoxRenderer();

			virtual void Awake();
			virtual void Init();
			virtual void Update();
			virtual bool Draw();

			HRESULT SetBox(Vector3 vBBox);
			inline void SetSize(Vector3 vBBox) { m_vSize = vBBox; }
			
#if BUILD_MODE

			void ImGuiDebug();

#endif // BUILD_MODE

		};
	}
}
CEREAL_REGISTER_TYPE(MySpace::Game::CBoxRenderer)

#endif // !__BOX_MESH_H__
